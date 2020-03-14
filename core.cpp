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

void Colidable::update()
{
	//TODO
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
