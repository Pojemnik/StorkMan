#include "physics.h"

void Physical::reset_physics()
{
	acceleration = { 0,0 };
	speed = { 0,0 };
}

Vectorf Physical::resolve_one_sided_collisions(Vectorf current_collision_vector, int id)
{
	float up = util::vector_dot_product({ 0,-1 }, current_collision_vector) /
		(std::hypot(current_collision_vector.x, current_collision_vector.y));
	if (!last_one_side_collision_ids.contains(id))
	{
		if (up < -0.7f && !fallthrough)
		{
			return current_collision_vector;
		}
		else
		{
			current_one_side_collision_ids.insert(id);
		}
	}
	else
	{
		current_one_side_collision_ids.insert(id);
	}
	return Vectorf(0, 0);
}

Physical::Physical(std::vector<Vectorf>&& mesh, Vectorf pos_) :
	collision(std::move(mesh), pos_), pos(pos_)
{
	set_defaults();
}

Physical::Physical(std::vector<Vectorf> mesh, Vectorf pos_) :
	collision(mesh, pos_), pos(pos_)
{
	set_defaults();
}

const Collision* const Physical::get_collision() const
{
	return &collision;
}

Vectorf Physical::get_pos()
{
	return pos;
}

void Physical::update_physics(float dt)
{
	temp_delta = -temp_delta;
	delta_pos += move_delta * dt;
	speed += acceleration * dt;
	if (temp_delta != Vectorf(0, 0))
	{
		float k = util::vector_dot_product(speed, temp_delta) /
			util::vector_dot_product(temp_delta, temp_delta);
		if (k < 0)
		{
			if (surface == 3)
			{
				speed -= temp_delta * k;
				speed *= 0.9f;
			}
			else
			{
				if (abs(temp_delta.x) > 0.0001 && temp_delta.x * speed.x < 0)
				{
					speed.x = 0;
				}
				if (abs(temp_delta.y) > 0.1 && temp_delta.y * speed.y < 0)
				{
					speed.y = 0;
				}
			}
		}
		delta_pos += temp_delta - util::normalize(temp_delta, 2.0f);
	}
	delta_pos += speed * dt;
	delta_pos += external_speed * dt;
	pos += delta_pos;
	collision.rect.left += delta_pos.x;
	collision.rect.top += delta_pos.y;
	for (auto& it : collision.mesh)
	{
		it += delta_pos;
	}
	last_one_side_collision_ids = current_one_side_collision_ids;
	current_one_side_collision_ids.clear();
	last_on_ground = on_ground;
	on_ground = (max_up < 0);
	collision.calculate_min_max_arr();
	set_defaults();
}

void Physical::set_defaults()
{
	acceleration = Vectorf(0, 0);
	temp_delta = Vectorf(0, 0);
	delta_pos = Vectorf(0, 0);
	move_delta = Vectorf(0, 0);
	external_speed = Vectorf(0, 0);
	surface = 0;
	max_up = 1.f;
}

void Physical::apply_force(Vectorf force_)
{
	acceleration += force_;
}

void Physical::resolve_collision(const std::vector<std::shared_ptr<const Collidable>>& others)
{
	for (const auto& it : others)
	{
		const Collision* const other_collision = it->get_collision();
		if (other_collision == &collision)
			continue;
		if (!collision.rect.intersects(other_collision->rect))
			continue;
		Vectorf current_collision_vector = coll::test_collision(collision, *other_collision);
		if (!other_collision->one_sided)
		{
			temp_delta += current_collision_vector;
		}
		else
		{
			if (!util::round_and_compare(current_collision_vector, Vectorf(0, 0), 0.1f))
			{
				temp_delta += resolve_one_sided_collisions(current_collision_vector, other_collision->id);
			}
		}
		if (temp_delta.y > 0)
		{
			float up = util::vector_dot_product({ 0,-1 }, temp_delta) /
				(std::hypot(temp_delta.x, temp_delta.y));
			if (up < max_up)
			{
				max_up = up;
				surface = it->get_collision()->surface;
				external_speed = it->get_speed();
			}
		}
	}
}

void Physical::resolve_collision(const Collidable& other)
//Unused. Why?
{
	const Collision* const other_collision = other.get_collision();
	if (other_collision == &collision)
		return;
	if (!collision.rect.intersects(other_collision->rect))
		return;
	Vectorf current_collision_vector = coll::test_collision(collision, *other_collision);
	if (!other_collision->one_sided)
	{
		temp_delta += current_collision_vector;
	}
	else
	{
		if (!util::round_and_compare(current_collision_vector, Vectorf(0, 0), 0.1f))
		{
			temp_delta += resolve_one_sided_collisions(current_collision_vector, other_collision->id);
		}
	}
	if (temp_delta.y > 0)
	{
		float up = util::vector_dot_product({ 0,-1 }, temp_delta) /
			(std::hypot(temp_delta.x, temp_delta.y));
		if (up < max_up)
		{
			max_up = up;
			surface = other.get_collision()->surface;
			external_speed = other.get_speed();
		}
	}
}

void Physical::move(Vectorf delta)
{
	move_delta += delta;
}

void Physical::set_position(Vectorf new_pos)
{
	delta_pos = new_pos - pos;
	reset_physics();
}

std::pair<Vectorf, int> Physical::get_collision_info() const
{
	return std::make_pair(collision_vector, surface);
}

bool Physical::is_on_ground() const
{
	return on_ground || last_on_ground;
}

sf::FloatRect Physical::get_bounding_rect() const
{
	return collision.rect;
}

void Physical::set_fallthrough(bool val)
{
	fallthrough = val;
}
