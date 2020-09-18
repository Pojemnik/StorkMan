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
	virtual void move(Vectorf delta) = 0;
	virtual void set_position(Vectorf new_position) = 0;
};

class Renderable : public sf::Drawable
{
};