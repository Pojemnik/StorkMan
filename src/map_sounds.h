#pragma once
#include <SFML/Audio.hpp>
#include "collisions.h"

class Map_sound
{
	const int sound;
	Collision collision;
	int volume;
	const int id;
	Vectorf pos;
	//Other sound params

public:
	Map_sound(int sound_, Collision&& collision_, int volume_, int id_);
	Collision get_collision() const;
	int get_sound() const;
	int get_id() const;
	Vectorf get_pos() const;
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