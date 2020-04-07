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

sf::Vector2f Physical::uncolide(const Colidable* c, float dt)
{
	if (rect_collision.intersects(c->rect_collision))
	{
		sf::Vector2f tmp = test_collision(&mesh.vertices, &c->mesh.vertices);
		total_speed += tmp * -1.0f;
		colision_direction.x = util::sgn(tmp.x);
		colision_direction.y = util::sgn(tmp.y);
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

Physical::Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m) : Colidable(rect, mesh, t), mass(m)
{

}

Colidable::Colidable(sf::FloatRect rect, std::vector<Vectorf> _mesh, Colidable_type t) : rect_collision(rect), type(t)
{
	mesh.vertices = _mesh;
}

bool Physical::test_colision(const Colidable& other)
{
	if (!rect_collision.intersects(other.rect_collision))
		return false;
	return test_bollean(&mesh.vertices, &other.mesh.vertices);
}