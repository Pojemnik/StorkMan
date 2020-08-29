#include "physics.h"

Mesh_collision::Mesh_collision(sf::FloatRect rect)
{
	vertices = std::vector<Vectorf>();
	vertices.push_back({ rect.left, rect.top });
	vertices.push_back({ rect.left + rect.width, rect.top });
	vertices.push_back({ rect.left + rect.width, rect.top + rect.height });
	vertices.push_back({ rect.left, rect.top + rect.height });
}

Mesh_collision::Mesh_collision(sf::FloatRect rect, float scale, Vectorf pos)
{
	vertices = std::vector<Vectorf>();
	vertices.push_back({ rect.left * scale + pos.x, rect.top * scale + pos.y });
	vertices.push_back({ rect.left * scale + pos.x + rect.width * scale, rect.top * scale + pos.y });
	vertices.push_back({ rect.left * scale + pos.x + rect.width * scale, rect.top * scale + pos.y + rect.height * scale });
	vertices.push_back({ rect.left * scale + pos.x, rect.top * scale + pos.y + rect.height * scale });
}

void Physical::apply_force(Vectorf f)
{
	force += f;
}

sf::Vector2f Physical::uncollide(const Collidable* c, float dt)
{
	if (rect_collision.intersects(c->rect_collision))
	{
		sf::Vector2f tmp = coll::test_collision(&mesh.vertices, &c->mesh.vertices);
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

sf::Vector2f Physical::uncollide(Physical* p, float dt)
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

Physical::Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Collidable_type t, float m)
	: Collidable(rect, mesh, t), mass(m)
{

}


Collidable::Collidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Collidable_type t)
	: rect_collision(rect), type(t)
{
	mesh.vertices = _mesh;
}

void Collidable::rescale(float ratio)
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

bool Physical::test_collision(const Collidable& other)
{
	if (!rect_collision.intersects(other.rect_collision))
		return false;
	return coll::test_bollean(&mesh.vertices, &other.mesh.vertices);
}