#pragma once
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>

#include "util.h"

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;
typedef std::string string;

class Polygon_generator
{
	static const int eps = 0.001f;
	static const int polygon_const = 2.f;
	static std::pair<float, Vectorf>cast_ray(Vectorf source, Vectorf alfa,
		std::vector<std::pair<Vectorf, Vectorf>>& map_edges);
	static int add_bounds(Vectorf source, float max_size, std::vector<std::pair<Vectorf,
		Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices);
	static void remove_bounds(int number, std::vector<std::pair<Vectorf,
		Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices);
	static std::vector<Vectorf> simplify(std::vector<Vectorf>&& polygon);

public:
	static std::vector<Vectorf> calc_polygon(Vectorf source,
		float max_size,
		std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices);
};