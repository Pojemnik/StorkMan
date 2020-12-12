#include "collisions.h"

Collision::Collision(sf::FloatRect rect_)
{
	mesh = std::vector<Vectorf>();
	mesh.push_back({ rect_.left, rect_.top });
	mesh.push_back({ rect_.left + rect_.width, rect_.top });
	mesh.push_back({ rect_.left + rect_.width, rect_.top + rect_.height });
	mesh.push_back({ rect_.left, rect_.top + rect_.height });
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(std::vector<Vectorf>&& mesh_) : mesh(mesh_)
{
	rect = util::mesh_to_rect(mesh);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(std::vector<Vectorf>&& mesh_, Vectorf pos) : mesh(mesh_)
{
	for (auto& it : mesh)
	{
		it += pos;
	}
	rect = util::mesh_to_rect(mesh);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos) : mesh(vertices.size())
{
	int i = 0;
	for (const auto& it : vertices)
	{
		mesh[i++] = it.position + pos;
	}
	rect = util::mesh_to_rect(mesh);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(const std::vector<Vectorf>& vertices, Vectorf pos) : mesh(vertices)
{
	for (auto& it : mesh)
	{
		it += pos;
	}
	rect = util::mesh_to_rect(mesh);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(sf::FloatRect rect_, float scale, Vectorf pos)
{
	mesh = std::vector<Vectorf>();
	mesh.push_back({ rect_.left * scale + pos.x, rect_.top * scale + pos.y });
	mesh.push_back({ rect_.left * scale + pos.x + rect_.width * scale, rect_.top * scale + pos.y });
	mesh.push_back({ rect_.left * scale + pos.x + rect_.width * scale, rect_.top * scale + pos.y + rect_.height * scale });
	mesh.push_back({ rect_.left * scale + pos.x, rect_.top * scale + pos.y + rect_.height * scale });
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(std::vector<Vectorf>&& mesh_, Vectorf pos, int surface_) :
 surface(surface_)
{
	for (auto& it : mesh_)
	{
		it += pos;
	}
	rect = util::mesh_to_rect(mesh_);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

Collision::Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos, int surface_) :
	mesh(vertices.size()), surface(surface_)
{
	int i = 0;
	for (const auto& it : vertices)
	{
		mesh[i++] = it.position + pos;
	}
	rect = util::mesh_to_rect(mesh);
	calculate_min_max_arr();
	id = context.id_generator.get_id();
}

void Collision::calculate_min_max_arr()
{
	min_max_arr.clear();
	for (int i = 0; i < mesh.size(); i++)
	{
		Vectorf vAxis = util::get_axis_normal(mesh, i);
		float max0 = util::vector_dot_product(vAxis, mesh[0]);
		float min0 = max0;
		for (int j = 1; j < mesh.size(); j++)
		{
			float t = util::vector_dot_product(vAxis, mesh[j]);
			if (t < min0)
				min0 = t;
			if (t > max0)
				max0 = t;
		}
		min_max_arr.emplace_back(min0, max0);
	}
}

bool coll::test_bollean(const Collision& a, const Collision& b)
{
	return check_polygons_for_SAT(a, b, 0, 0).collides &&
		check_polygons_for_SAT(b, a, 1, 0).collides;
}

Collision_info coll::check_polygons_for_SAT(const Collision& a,
	const Collision& b, bool flip, bool docalc)
{
	Collision_info result(true);
	float shortestDist = FLT_MAX;
	for (int i = 0; i < a.mesh.size(); i++)
	{
		Vectorf vAxis = util::get_axis_normal(a.mesh, i);
		float max1 = util::vector_dot_product(vAxis, b.mesh[0]);
		float min1 = max1;
		for (int j = 1; j < b.mesh.size(); j++)
		{
			float t = util::vector_dot_product(vAxis, b.mesh[j]);
			if (t < min1)
				min1 = t;
			if (t > max1)
				max1 = t;
		}
		if (a.min_max_arr[i].first > max1 || min1 > a.min_max_arr[i].second)
			return Collision_info(false);
		if (docalc)
		{
			float distmin = a.min_max_arr[i].first - max1;
			if (flip)
			{
				distmin *= -1;
			}
			float distminABS = fabs(distmin);
			if (distminABS < shortestDist)
			{
				result.distance = distmin;
				result.vector = vAxis;
				shortestDist = distminABS;
			}
		}
	}
	return result;
}

sf::Vector2f coll::test_collision(const Collision& a, const Collision& b)
{
	Collision_info result1 = check_polygons_for_SAT(a, b, 0, 1);
	if (!result1.collides)
		return sf::Vector2f(0, 0);
	Collision_info result2 = check_polygons_for_SAT(b, a, 1, 1);
	if (!result2.collides)
		return sf::Vector2f(0, 0);
	if (fabs(result1.distance) < fabs(result2.distance))
		return result1.vector * result1.distance;
	else
		return result2.vector * result2.distance;

}

Collision_info::Collision_info(bool c) : collides(c) {}
