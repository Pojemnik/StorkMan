#pragma once

#include "core.h"

class Platform : public Texturable, public Colidable
{
public:
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
};

class Entity : public Animatable, public Colidable, public Transformable
{
private:
	std::vector<const Animation*> animations;
	Vectorf move_delta;
public:
	Entity_status status;
	void set_animation(const Animation* t);
	Entity(Vectorf p, std::vector<const Animation*> t, float h, float gs, float m);
	void move(Vectorf delta);
	void next_frame();
};

class Player : public Entity
{
public:
	Player(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m);
};