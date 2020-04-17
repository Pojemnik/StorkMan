#pragma once
#include "util.h"
#include "collisions.h"

class Mesh_collision
{
public:
	std::vector<Vectorf> vertices;

	Mesh_collision() = default;
	Mesh_collision(sf::FloatRect rect);
	Mesh_collision(sf::FloatRect rect, float scale, Vectorf pos);
};

class Colidable
{
public:
	sf::FloatRect rect_collision;
	Mesh_collision mesh;
	Colidable_type type;
	Colidable() = default;
	Colidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Colidable_type t);

};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
};

class Physical : public Transformable, public Colidable
{
protected:
	float mass;
	Vectorf total_speed = { 0,0 };
	Vectorf move_speed = { 0,0 };
	Vectorf last_speed = { 0,0 };
	Vectorf force = { 0,0 };
	Vectorf move_force = { 0,0 };
	Vectori colision_direction = { 0,0 };
	
	virtual void update_position(float dt) = 0;
public:
	Vectorf maxcollisionvector = {0,0};
	virtual void update(float dt) = 0;
	virtual void move(Vectorf delta) = 0;
	virtual void apply_force(Vectorf f);
	sf::Vector2f uncolide(const Colidable* c, float dt);
	bool test_colision(const Colidable& other);
	Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m);
	Physical() = default;
};
