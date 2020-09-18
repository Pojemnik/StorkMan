#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cfloat>

#include "util.h"

struct Collision
{
	std::vector<Vectorf> mesh;
	sf::FloatRect rect;
	std::vector<std::pair<float, float>> min_max_arr;

	Collision() = default;
	Collision(sf::FloatRect rect_);
	Collision(std::vector<Vectorf> mesh_);
	Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos);
	Collision(sf::FloatRect rect_, float scale, Vectorf pos);
	void calculate_min_max_arr();
};

struct Collision_info
{
	bool collides = true;
	sf::Vector2f vector;
	float distance = .0f;

	Collision_info(bool);
	Collision_info() = default;
};

namespace coll
{
	bool test_bollean(const Collision& a,
		const Collision& b);
	Collision_info check_polygons_for_SAT(const Collision& a,
		const Collision& b, bool flip, bool docalc);
	sf::Vector2f test_collision(const Collision& a,
		const Collision& b);
}