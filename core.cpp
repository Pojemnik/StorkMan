#include "core.h"

template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

Renderable::Renderable(Vectorf p, std::shared_ptr<sf::Texture> t, float h) : tex(t), pos(p), height(h)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Texturable::Texturable(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<sf::Vertex> v) : tex(t), pos(p), vertices(v)
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

Animatable::Animatable(Vectorf p, std::shared_ptr<std::vector<sf::Texture>> t, float h, float gs) : tex(t), pos(p), height(h)
{
	it = tex->begin();
	sprite = sf::Sprite(*it);
	sprite.setPosition(pos);
	float scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

void Animatable::next_frame()
{
	if (++it == tex->end())
		it = tex->begin();
	sprite.setTexture(*it);
}

void Animatable::set_animation(std::shared_ptr<std::vector<sf::Texture>> t)
{
	tex = t;
	it = tex->begin();
}

Platform::Platform(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{

}

Player::Player(Vectorf p, std::vector<std::shared_ptr<std::vector<sf::Texture>>> t, float h, float gs) : Entity(p, t, h, gs)
{

}

Entity::Entity(Vectorf p, std::vector<std::shared_ptr<std::vector<sf::Texture>>> t, float h, float gs) : Animatable(p, t[0], h, gs), animations(t)
{

}

void Entity::move(Vectorf delta)
{
	int s = sgn(delta.x);
	if (direction != s)
	{
		Vectorf tmp = sprite.getScale();
		tmp.x *= -1;
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
	status = Entity_status::MOVE;
}

void Entity::next_frame()
{
	if (&*tex != &*animations[status])
	{
		set_animation(animations[status]);
	}
	if (++it == tex->end())
		it = tex->begin();
	sprite.setTexture(*it);
}
