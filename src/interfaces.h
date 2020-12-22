#pragma once
#include <SFML/Graphics.hpp>

class Map_object
{
public:
	virtual sf::FloatRect get_bounding_rect() const = 0;
};

class Physical_updatable
{
public:
	virtual void update_physics(float dt) = 0;
};
class Graphical_updatable
{
public:
	virtual void update_graphics(float dt) = 0;
};

class Renderable : public sf::Drawable
{
	void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class Interactive
{
public:
	virtual std::vector<int> get_events() = 0;
	virtual void clicked(Vectori mouse_pos, int button) = 0;
};