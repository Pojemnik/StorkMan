#pragma once
#include "graphics.h"
#include "physics.h"

class Platform : public Texturable, public Colidable
{
public:
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
	void rescale(float ratio);
};

class Entity : public Animatable, public Physical
{
private:
	std::vector<const Animation*> animations;
	bool reset_animation = false;

	void update_position();

public:
	Entity_status animation_status;
	void set_animation(const Animation* t);
	Entity(Vectorf p, std::vector<const Animation*> t, float h, float gs, float m);
	void move(Vectorf delta);
	void jump();
	void next_frame();
	void update(float dt);
	Vectorf get_position();
};

class Dynamic_entity : public Dynamic_animatable, public Physical
{
protected:
	bool reset_animation = false;

	void update_position(float dt);
	void flip(int s);
	void set_idle();

public:
	Dynamic_entity(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v, std::vector<const Dynamic_animation*> a, sf::FloatRect rc, Animation_tree t, float h, float gs, float m);
	void move(Vectorf delta);
	void jump(bool run);
	void update(float dt);
	void next_frame();
	Vectorf get_position();
	void rescale(float new_scale);
};

class Player : public Dynamic_entity
{
public:
	Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v, std::vector<const Dynamic_animation*> a, sf::FloatRect rc, Animation_tree t, float h, float gs, float m);
	void attack(int type);
};