#include "core.h"

template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

Renderable::Renderable(Vectorf p, sf::Texture* t, float h) : tex(t), pos(p), height(h)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Texturable::Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : tex(t), pos(p), vertices(points)
{
	shape = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Stream);
	shape.create(vertices.size());
	shape.update(&vertices[0]);
}

void Texturable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

void Renderable::rescale(float gs)
{
	float scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

Animatable::Animatable(Vectorf p, const Animation* t, float h, float gs) : tex(t), pos(p), height(h)
{
	it = tex->begin();
	sprite = sf::Sprite(*it);
	sprite.setPosition(pos);
	scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

void Animatable::next_frame()
{
	if (++it == tex->end())
		it = tex->begin();
	sprite.setTexture(*it);
}

void Entity::set_animation(const Animation* t)
{
	this->pos += -(t->center - tex->center)*this->scale;
	this->sprite.move(-(t->center - tex->center)*this->scale);
	//std::cout << pos.x << ' ' << pos.y << std::endl;
	tex = t;
	it = tex->begin();
}

void Animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Platform::Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{

}

Player::Player(Vectorf p, std::vector<const Animation* > t, float h, float gs) : Entity(p, t, h, gs)
{

}

Entity::Entity(Vectorf p, std::vector<const Animation* > t, float h, float gs) : Animatable(p, t[0], h, gs), animations(t)
{
	status = Entity_status::IDLE;
}

void Entity::move(Vectorf delta)
{
	if (status == Entity_status::IDLE)
	{
		status = Entity_status::MOVE;
		next_frame();
	}
	int s = sgn(delta.x);
	if (direction != s)
	{
		Vectorf tmp = sprite.getScale();
		tmp.x *= -1;
		scale = -scale;
		if (s == -1)
		{
			sprite.setOrigin(sprite.getLocalBounds().width, 0);
			sprite.setScale(-1, 1);
		}
		else
		{
			sprite.setOrigin(0, 0);
			sprite.setScale(1, 1);
		}
		sprite.setScale(tmp);
	}
	direction = s;
	pos += delta;
	sprite.setPosition(pos);
}

void Entity::next_frame()
{
	if (&*tex != &*animations[status])
	{
		set_animation(animations[status]);
		sf::Vector2u size = it->getSize();
		sprite.setTextureRect(sf::IntRect(0,0,size.x,size.y));
	}
	if (++it == tex->end())
	{
		if (status != Entity_status::MOVE)
		{
			status = Entity_status::IDLE;
			set_animation(animations[status]);
		}
		else
			it = tex->begin();
	}	
	sprite.setTexture(*it);
}

Animation::Animation(std::vector<sf::Texture> &a, Vectorf c) : content(a), center(c)
{
}

std::vector<sf::Texture>::const_iterator Animation::begin() const
{
	return content.begin();
}

std::vector<sf::Texture>::const_iterator Animation::end() const
{
	return content.end();
}
