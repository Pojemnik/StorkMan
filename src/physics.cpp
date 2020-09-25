#include "physics.h"

void Physical::reset_physics()
{
	acceleration = { 0,0 };
	speed = { 0,0 };
}

Physical::Physical(std::vector<Vectorf>&& mesh, Vectorf pos_) : collision(std::move(mesh)),
pos(pos_), acceleration(0, 0), speed(0, 0) {}

const Collision* const Physical::get_collision() const
{
	return &collision;
}

Vectorf Physical::get_pos()
{
	return pos;
}

void Physical::update(float dt)
{
	speed += acceleration * dt;
	delta_pos += speed * dt;
	pos += delta_pos;
	collision.rect.left += delta_pos.x;
	collision.rect.top += delta_pos.y;
	for (auto& it : collision.mesh)
	{
		it += delta_pos;
	}
	acceleration = { 0,0 };
	delta_pos = { 0,0 };
	on_ground = false;
	if (temp_delta != Vectorf(0, 0))
	{
		float k = util::vector_dot_product(speed, temp_delta) /
			util::vector_dot_product(temp_delta, temp_delta);
		if (k > 0)
			speed -= temp_delta * k;
		delta_pos += temp_delta;
	}
	collision_vector = temp_delta;
	if (util::vector_dot_product({ 0,-1 }, collision_vector) /
		(std::hypot(collision_vector.x, collision_vector.y)) > 0.5f)
	{
		on_ground - true;
	}
	surface = Surface_type::NONE;
	max_up = -1.f;
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
		temp_delta += coll::test_collision(collision, *other_collision);
		if (temp_delta.y > 0)
		{
			float up = util::vector_dot_product({ 0,-1 }, temp_delta) /
				(std::hypot(temp_delta.x, temp_delta.y));
			if (up > max_up)
			{
				max_up = up;
				surface = it->get_collision()->surface;
			}
		}
	}
}

void Physical::resolve_collision(const Collidable& other)
{
	const Collision* const other_collision = other.get_collision();
	if (other_collision == &collision)
		return;
	if (!collision.rect.intersects(other_collision->rect))
		return;
	temp_delta += coll::test_collision(collision, *other_collision);
	if (temp_delta.y > 0)
	{
		float up = util::vector_dot_product({ 0,-1 }, temp_delta) /
			(std::hypot(temp_delta.x, temp_delta.y));
		if (up > max_up)
		{
			max_up = up;
			surface = other.get_collision()->surface;
		}
	}
}

void Physical::move(Vectorf delta)
{
	delta_pos += delta;
}

void Physical::set_position(Vectorf new_pos)
{
	delta_pos = new_pos - pos;
	reset_physics();
}

std::pair<Vectorf, Surface_type> Physical::get_collision_info() const
{
	return std::make_pair(collision_vector, surface);
}

bool Physical::is_on_ground() const
{
	return on_ground;
}

sf::FloatRect Physical::get_bounding_rect() const
{
	return collision.rect;
}
