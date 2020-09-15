#pragma once
#include "util.h"
#include "collisions.h"

struct Collision
{
	std::vector<Vectorf> mesh;
	sf::FloatRect rect;

	Collision() = default;
	Collision(sf::FloatRect rect_);
	Collision(std::vector<Vectorf> mesh_);
	Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos);
	Collision(sf::FloatRect rect_, float scale, Vectorf pos);
};

class Collidable
{
public:
	virtual const Collision* const get_collision() = 0;
};

class old_Collidable
{
public:
	sf::FloatRect rect_collision;
	Collision mesh;
	Collidable_type type;
	old_Collidable() = default;
	old_Collidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Collidable_type t);
	void rescale(float ratio);
};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
	virtual void set_position(Vectorf new_position) = 0;
};

class Physical : public Transformable, public old_Collidable
{
protected:
	float MIN_EXTERNAL_SPEED = 1;
	float mass = 0;
	Vectorf total_speed = { 0,0 };
	Vectorf move_speed = { 0,0 };
	Vectorf force = { 0,0 };
	Vectorf move_force = { 0,0 };
	Vectori collision_direction = { 0,0 };
	Vectorf external_speed = { 0,0 };
	enum class Run_block { LEFT, RIGHT, NONE } run_block = Run_block::NONE;

	virtual void update_position(float dt) = 0;
public:
	Vectorf maxcollisionvector = { 0,1 };
	virtual void update(float dt) = 0;
	virtual void move(Vectorf delta) = 0;
	virtual void apply_force(Vectorf f);
	sf::Vector2f uncollide(const old_Collidable* c, float dt);
	sf::Vector2f uncollide(Physical* c, float dt);
	bool test_collision(const old_Collidable& other);
	Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Collidable_type t,
		float m);
	Physical() = default;
};
