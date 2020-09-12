#include "graphics.h"

const Vectorf fliptab[] = { {1,1},{-1,1},{1,-1},{-1,-1} };

old_Renderable::old_Renderable(Vectorf p, const sf::Texture* t, float h, int l)
	: tex(t), pos(p), height(h), layer(l)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
	rescale(1);
}

old_Renderable::old_Renderable(
	Vectorf p, const sf::Texture* t, float h, int l, int flip, float ang) 
	: tex(t), pos(p), height(h), layer(l), flipint(flip)
{

	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
	sprite.setRotation(ang);
	rescale(1);
}

void old_Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void old_Renderable::rescale(float ratio)
{
	float scale = ratio * 32 * height / sprite.getTexture()->getSize().y;
	pos *= ratio;
	sprite.setPosition(pos);
	sprite.setScale(fliptab[flipint]*scale);
}

old_Animatable::old_Animatable(
	Vectorf p, const std::vector<sf::Texture>* a, float h, int l) :
	animation(a), pos(p), height(h), layer(l)
{
	it = animation->begin();
	sprite.setTexture(*it);
	sprite.setPosition(pos);
	rescale(1);
}

old_Animatable::old_Animatable(Vectorf p, const std::vector<sf::Texture>* a, float h,
	int l, int flip, float ang)
	: animation(a), pos(p), height(h), layer(l), flipint(flip)
{
	it = animation->begin();
	sprite.setTexture(*it);
	sprite.setPosition(pos);
	sprite.setRotation(ang);
	rescale(1);
}

void old_Animatable::next_frame()
{
	if (++it == animation->end())
	{
		it = animation->begin();
	}
	sprite.setTexture(*it);
}

void old_Animatable::rescale(float ratio)
{
	float scale = ratio * 32 * height / sprite.getTexture()->getSize().y;
	pos *= ratio;
	sprite.setPosition(pos);
	sprite.setScale(fliptab[flipint] * scale);
}

void old_Animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Texturable::Texturable(Vectorf p, const sf::Texture* t,
	std::vector<sf::Vertex> points, int l)
	: tex(t), pos(p), vertices(points), layer(l)
{
	shape = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Stream);
	shape.create(vertices.size());
	shape.update(&vertices[0]);
}

void Texturable::rescale(float ratio)
{
	pos *= ratio;
	for(auto& it : vertices)
	{
		it.position *= ratio;
	}
	shape.update(&vertices[0]);
}

void Texturable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}