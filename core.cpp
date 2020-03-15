#include "core.h"

Renderable::Renderable(Vectorf p, sf::Texture* t, float h) : tex(t), pos(p), height(h)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Renderable::rescale(float gs)
{
	float scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
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

void Animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Animation::Animation(std::vector<sf::Texture>& a, Vectorf c) : content(a), center(c) {}

std::vector<sf::Texture>::const_iterator Animation::begin() const
{
	return content.begin();
}

std::vector<sf::Texture>::const_iterator Animation::end() const
{
	return content.end();
}

void Physical::apply_force(Vectorf f)
{
	force += f;
}

void Physical::uncolide(const Colidable* c)
{
	Vectorf tab[6] = { {0, -0.6f},  {0, -0.6f}, {-0.6f, 1.2}, {-0.6f, 0}, {0, -0.6f}, {0, -0.6f} };
	sf::FloatRect intersection;
	int i = 0;
	for (i = 0; i < 6 && rect_collision.intersects(c->rect_collision, intersection); i++)
	{
		move({ last_move_delta.x * tab[i].x, last_move_delta.y * tab[i].y });
		update_position();
	}
	if (i != 0)
	{
		colision_direction = { sgn(-last_move_delta.x * tab[i - 1].x), sgn(-last_move_delta.y * tab[i - 1].y) };
		if (colision_direction.x)
			force.x = 0;
		if (colision_direction.y)
			force.y = 0;
	}
}

Physical::Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m) : Colidable(rect, mesh, t), mass(m)
{

}

Colidable::Colidable(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t) : rect_collision(rect), mesh_collision(mesh), type(t)
{

}
