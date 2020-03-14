#pragma once

#include "core.h"

class Platform : public Texturable, public Colidable
{
public:
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
};

class Entity : public Animatable, public Physical
{
private:
	std::vector<const Animation*> animations;

public:
	Entity_status status;
	void set_animation(const Animation* t);
	Entity(Vectorf p, std::vector<const Animation*> t, float h, float gs, float m);
	void move(Vectorf delta);
	void next_frame();
	void update();
};

class Player : public Entity
{
public:
	Player(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m);
};