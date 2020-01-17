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

Object::Object(Vectorf pos, std::shared_ptr<sf::Texture> t)
{

}