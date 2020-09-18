#pragma once
#include "util.h"
#include "collisions.h"
#include "logic.h"

class Collidable
{
public:
	virtual const Collision* const get_collision() const = 0;
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

class Physical : public Updatable, public Collidable
{
	Collision collision;
	Vectorf acceleration;
	Vectorf speed;
	Vectorf pos;
	Vectorf delta_pos;

public:
	Physical(std::vector<Vectorf>&& mesh, Vectorf pos_);
	const Collision* const get_collision() const;
	Vectorf get_pos();
	void update(float dt);
	void apply_force(Vectorf force_);
	void resolve_collision(const std::vector<const Collidable*>& others);
	void move(Vectorf delta);
	void set_position(Vectorf new_pos);
};

class old_Physical : public Transformable, public old_Collidable
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
	sf::Vector2f uncollide(old_Physical* c, float dt);
	bool test_collision(const old_Collidable& other);
	old_Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Collidable_type t,
		float m);
	old_Physical() = default;
};
