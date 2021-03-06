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
	int surface = 0;
	int id = -1;
	bool one_sided = false;

	Collision(sf::FloatRect rect_);
	Collision(std::vector<Vectorf>&& mesh_);
	Collision(std::vector<Vectorf>&& mesh_, Vectorf pos);
	Collision(std::vector<Vectorf>&& mesh_, Vectorf pos, int surface_);
	Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos);
	Collision(const std::vector<sf::Vertex>& vertices, Vectorf pos, int surface_);
	Collision(const std::vector<Vectorf>& vertices, Vectorf pos);
	Collision(sf::FloatRect rect_, float scale, Vectorf pos);
	void calculate_min_max_arr();
};

class Collidable
{
public:
	virtual const Collision* const get_collision() const = 0;
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const { (void)target; (void)states; }
	virtual Vectorf get_speed() const { return Vectorf(0, 0); }
};

struct Collision_info
{
	bool collides = true;
	sf::Vector2f vector = { 0,0 };
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