#pragma once
#include "util.h"
#include "collisions.h"
#include "interfaces.h"

class Physical : public Updatable, public Collidable
{
	Collision collision;
	Vectorf acceleration;
	Vectorf speed;
	Vectorf pos;
	Vectorf delta_pos;
	Vectorf collision_vector;
	Surface_type surface;
	bool on_ground = false;

public:
	Physical(std::vector<Vectorf>&& mesh, Vectorf pos_);
	const Collision* const get_collision() const;
	Vectorf get_pos();
	void update(float dt);
	void apply_force(Vectorf force_);
	void resolve_collision(const std::vector<const Collidable*>& others);
	void move(Vectorf delta);
	void set_position(Vectorf new_pos);
	std::pair<Vectorf, Surface_type> get_collision_info() const;
	bool is_on_ground() const;
};