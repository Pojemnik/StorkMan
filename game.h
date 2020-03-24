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
	bool reset_animation = false;

public:
	Entity_status animation_status;
	void set_animation(const Animation* t);
	Entity(Vectorf p, std::vector<const Animation*> t, float h, float gs, float m);
	void move(Vectorf delta);
	void jump();
	void next_frame();
	void update();
	void update_position();
	Vectorf get_position();
};

class Dynamic_entity : public Dynamic_animatable, public Physical
{
private:
	bool reset_animation = false;

public:
	Dynamic_entity(Vectorf p, std::vector<sf::Texture*>& v, std::vector<const Dynamic_animation*> a, sf::FloatRect rc, float h, float gs, float m);
	void move(Vectorf delta);
	void jump(bool run);
	void update();
	void update_position();
	void next_frame();
	Vectorf get_position();
};

class Player : public Dynamic_entity
{
public:
	Player(Vectorf p, std::vector<sf::Texture*>& v, std::vector<const Dynamic_animation*> a, sf::FloatRect rc, float h, float gs, float m);
};