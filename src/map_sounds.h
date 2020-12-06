#pragma once
#include <SFML/Audio.hpp>
#include "collisions.h"
#include "map_sound_info.h"
#include "polygon_generator.h"

class Map_sound
{
	Collision collision;
	const Map_sound_info info;

public:
	Map_sound(std::vector<Vectorf>&& mesh, Map_sound_info info_);
	void update_collision(std::vector<std::pair<Vectorf, Vectorf>>& map_edges,
		std::vector<Vectorf>& map_vertices);
	Collision get_collision() const;
	int get_sound() const;
	int get_id() const;
	Vectorf get_pos() const;
	int get_default_volume() const;
	Map_sound_info get_info() const;
};

inline bool operator==(const Map_sound& lhs, const Map_sound& rhs)
{
	return lhs.get_id() == rhs.get_id();
}

inline bool operator!=(const Map_sound& lhs, const Map_sound& rhs)
{
	return !(lhs == rhs);
}

struct Map_sound_compare
{
	bool operator()(Map_sound const* lhs, Map_sound const* rhs) const
	{
		return lhs->get_id() == rhs->get_id();
	}
};