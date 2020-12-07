#pragma once
#include <iostream>
#include <set>

#include "collisions.h"
#include "interfaces.h"

class Physical : public Physical_updatable, public Collidable
{
	Collision collision;
	Vectorf acceleration;
	Vectorf speed;
	Vectorf external_speed;
	Vectorf pos;
	Vectorf delta_pos;
	Vectorf move_delta;
	Vectorf collision_vector;
	Surface_type surface;
	bool on_ground = false;
	bool last_on_ground = false;
	float max_up = 1.f;
	Vectorf temp_delta = { 0,0 };
	std::set<int> current_one_side_collision_ids;
	std::set<int> last_one_side_collision_ids;

	void reset_physics();
	Vectorf resolve_one_sided_collisions(Vectorf collision_vector, int id);

public:
	Physical(std::vector<Vectorf>&& mesh, Vectorf pos_);
	Physical(std::vector<Vectorf> mesh, Vectorf pos_);
	const Collision* const get_collision() const;
	Vectorf get_pos();
	void update_physics(float dt);
	void set_defaults();
	void apply_force(Vectorf force_);
	void resolve_collision(const std::vector<std::shared_ptr<const Collidable>>& others);
	void resolve_collision(const Collidable& other);
	void move(Vectorf delta);
	void set_position(Vectorf new_pos);
	std::pair<Vectorf, Surface_type> get_collision_info() const;
	bool is_on_ground() const;
	sf::FloatRect get_bounding_rect() const;
};