#include "physics.h"

void old_Physical::apply_force(Vectorf f)
{
	force += f;
}

sf::Vector2f old_Physical::uncollide(const old_Collidable* c, float dt)
{
	if (rect_collision.intersects(c->rect_collision))
	{
		sf::Vector2f tmp = coll::test_collision(&mesh.mesh, &c->mesh.vertices);
		total_speed += tmp * -1.0f;
		int sgnx = util::sgn(tmp.x);
		int sgny = util::sgn(tmp.y);
		float ang_sin = fabs(util::vector_cross_product(util::normalize(tmp), Vectorf(0, 1)));
		if (ang_sin > 0.8)
		{
			if (sgnx > 0)
			{
				run_block = Run_block::RIGHT;
			}
			else
			{
				run_block = Run_block::LEFT;
			}
		}
		if (sgnx != 0)
			collision_direction.x = sgnx;
		if (sgny != 0)
			collision_direction.y = sgny;
		if (abs(tmp.x) > 0.0001 && tmp.x * force.x > 0)
		{
			force.x = 0;
		}
		if (abs(tmp.y) > 0.0001 && tmp.y * force.y > 0)
		{
			force.y = 0;
		}
		update_position(dt);
		return tmp;
	}
	return { 0,0 };
}

sf::Vector2f old_Physical::uncollide(old_Physical* p, float dt)
{
	if (rect_collision.intersects(p->rect_collision))
	{
		sf::Vector2f tmp = coll::test_collision(&mesh.vertices, &p->mesh.vertices);
		if (tmp != Vectorf(0, 0))
		{
			if (p->mass == INFINITY && mass == INFINITY)
			{
				throw std::invalid_argument("B³¹d fizyki: zderzenie niesprê¿yste 2 cia³ o nieskoñczonych masach");
				// nie mam pojêcia
			}
			else if (p->mass == INFINITY)
			{
				external_speed = p->total_speed;
			}
			else if (mass == INFINITY)
			{
				p->external_speed = total_speed;
			}
			else
			{
				Vectorf u = (p->total_speed * p->mass + total_speed * mass) / (p->mass + mass);
				p->external_speed = u;
				external_speed = u;
			}
		}
		total_speed += tmp * -1.0f;
		int sgnx = util::sgn(tmp.x);
		int sgny = util::sgn(tmp.y);
		if (sgnx != 0)
			collision_direction.x = sgnx;
		if (sgny != 0)
			collision_direction.y = sgny;
		if (abs(tmp.x) > 0.0001 && tmp.x * force.x > 0)
		{
			force.x = 0;
		}
		if (abs(tmp.y) > 0.0001 && tmp.y * force.y > 0)
		{
			force.y = 0;
		}
		update_position(dt);
		return tmp;
	}
	return { 0,0 };
}

old_Physical::old_Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Collidable_type t, float m)
	: old_Collidable(rect, mesh, t), mass(m)
{

}


old_Collidable::old_Collidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Collidable_type t)
	: rect_collision(rect), type(t)
{
	mesh.vertices = _mesh;
}

void old_Collidable::rescale(float ratio)
{
	for (auto& it : mesh.vertices)
	{
		it *= ratio;
	}
	rect_collision.height *= ratio;
	rect_collision.width *= ratio;
	rect_collision.left *= ratio;
	rect_collision.top *= ratio;
}

bool old_Physical::test_collision(const old_Collidable& other)
{
	if (!rect_collision.intersects(other.rect_collision))
		return false;
	return coll::test_bollean(&mesh.vertices, &other.mesh.vertices);
}

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
