#include "physics.h"

Physical::Physical(std::vector<Vectorf>&& mesh, Vectorf pos_) : collision(std::move(mesh)),
pos(pos_), acceleration(0,0), speed(0,0)
{
}

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
}

void Physical::apply_force(Vectorf force_)
{
	acceleration += force_;
}

void Physical::resolve_collision(const std::vector<const Collidable*>& others)
{
	Vectorf delta(0,0);
	for (const auto& it : others)
	{
		const Collision* const other_collision = it->get_collision();
		if (other_collision == &collision)
			continue;
		if (!collision.rect.intersects(other_collision->rect))
			continue;
		delta += coll::test_collision(collision, *other_collision);
	}
	if (delta != Vectorf(0, 0))
	{
		float k = util::vector_dot_product(speed, delta) /
			util::vector_dot_product(delta, delta);
		if(k>0)
			speed -= delta * k;
		delta_pos += delta;
	}
}

void Physical::move(Vectorf delta)
{
	delta_pos += delta;
}

void Physical::set_position(Vectorf new_pos)
{
	delta_pos = new_pos - pos;
}
