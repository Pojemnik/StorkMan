#pragma once
#include <SFML/Graphics.hpp>

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
};