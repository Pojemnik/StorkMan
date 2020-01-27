#include "core.h"

Drawable::Drawable()
{

}

Drawable::Drawable(Vectorf p, std::shared_ptr<sf::Texture> t) : tex(t), pos(p)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Drawable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Texturable::Texturable(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<Vectorf> points) : tex(t), pos(p)
{
	shape = sf::ConvexShape(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		shape.setPoint(i, points[i]);
	}
	shape.setTexture(&*tex);	//Cudowne
	shape.setPosition(pos);
}

void Texturable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}


Platform::Platform(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<Vectorf> points) : Texturable(p, t, points)
{

}

Object::Object(Vectorf pos, std::shared_ptr<sf::Texture> t)
{

}