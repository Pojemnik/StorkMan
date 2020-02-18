#include "core.h"

Renderable::Renderable()
{

}

Renderable::Renderable(Vectorf p, std::shared_ptr<sf::Texture> t) : tex(t), pos(p)
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
	states.transform = sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

Animatable::Animatable(Vectorf p, std::shared_ptr<std::vector<sf::Texture>> t) : tex(t), pos(p)
{
	sprite.setPosition(pos);
	it = tex->begin();
	sprite = sf::Sprite(*it);
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

Platform::Platform(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{

}

Object::Object(Vectorf pos, std::shared_ptr<sf::Texture> t)
{

}