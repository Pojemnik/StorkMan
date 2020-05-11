#pragma once
#include "util.h"
#include "collisions.h"

struct Mesh_collision
{
public:
	std::vector<Vectorf> vertices;

	Mesh_collision() = default;
	Mesh_collision(sf::FloatRect rect);
	Mesh_collision(sf::FloatRect rect, float scale, Vectorf pos);
};

class Collidable
{
public:
	sf::FloatRect rect_collision;
	Mesh_collision mesh;
	Collidable_type type;
	Collidable() = default;
	Collidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Collidable_type t);
	void rescale(float ratio);
};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
};

class Physical : public Transformable, public Collidable
{
protected:
	float mass;
	Vectorf total_speed = { 0,0 };
	Vectorf move_speed = { 0,0 };
	Vectorf last_speed = { 0,0 };
	Vectorf force = { 0,0 };
	Vectorf move_force = { 0,0 };
	Vectori collision_direction = { 0,0 };
	
	virtual void update_position(float dt) = 0;
public:
	Vectorf maxcollisionvector = {0,0};
	virtual void update(float dt) = 0;
	virtual void move(Vectorf delta) = 0;
	virtual void apply_force(Vectorf f);
	sf::Vector2f uncollide(const Collidable* c, float dt);
	bool test_collision(const Collidable& other);
	Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Collidable_type t, float m);
	Physical() = default;
};
