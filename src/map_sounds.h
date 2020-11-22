#pragma once
#include <SFML/Audio.hpp>
#include "collisions.h"

class Map_sound
{
	const int sound;
	Collision collision;
	int default_volume;
	const int id;
	Vectorf pos;
	//Other sound params

public:
	Map_sound(int sound_, Vectorf pos_, std::vector<Vectorf>&& mesh, int volume_, int id_);
	Collision get_collision() const;
	int get_sound() const;
	int get_id() const;
	Vectorf get_pos() const;
	int get_default_volume() const;
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