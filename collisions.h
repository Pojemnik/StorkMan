#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "util.h"
struct Collision_info {
	bool shapeA_contained, shapeB_contained;
	bool collides=true;
	sf::Vector2f vector;
	float distance;

	Collision_info(bool);
	Collision_info() = default;
};

namespace coll
{
	bool test_bollean(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b);
	Collision_info check_polygons_for_SAT(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b, bool flip, bool docalc);
	sf::Vector2f test_collision(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b);
}