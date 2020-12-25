#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "collisions.h"
#include "map_sound_info.h"
#include "polygon_generator.h"

class Map_sound
{
	Collision collision;
	const Map_sound_info info;
	bool initialized = false;
	float max_x = -INFINITY;
	std::unordered_map<int, bool> events;
	bool enabled = true;

public:
	//Collision given by user
	Map_sound(std::vector<Vectorf>&& mesh, Map_sound_info info_,
		std::unordered_map<int, bool>&& events_);

	//Collision have to be generated
	Map_sound(Map_sound_info info_, std::unordered_map<int, bool>&& events_);
	void update_collision(std::vector<std::pair<Vectorf, Vectorf>>& map_edges,
		std::vector<Vectorf>& map_vertices);
	Collision get_collision() const;
	int get_id() const;
	Vectorf get_pos() const;
	Map_sound_info get_info() const;
	bool is_initialized() const;
	float get_max_x() const;
	bool update_and_get_state(std::vector<int> current_events);
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