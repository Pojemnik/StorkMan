#pragma once
#include <SFML/Graphics.hpp>

class Map_object
{
public:
	virtual sf::FloatRect get_bounding_rect() const = 0;
};

class Updatable
{
public:
	virtual void update(float dt) = 0;
};

class Transformable
{
public:
	virtual void move(sf::Vector2f delta) = 0;
	virtual void set_position(sf::Vector2f new_position) = 0;
};

class Renderable : public sf::Drawable
{
	void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};