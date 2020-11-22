#include "map_sounds.h"

Map_sound::Map_sound(int sound_, Vectorf pos_, std::vector<Vectorf>&& mesh, int volume_, int id_)
	: sound(sound_), collision(mesh, pos_), default_volume(volume_), id(id_), pos(pos_) {}

Collision Map_sound::get_collision() const
{
	return collision;
}

int Map_sound::get_sound() const
{
	return sound;
}

int Map_sound::get_id() const
{
	return id;
}

Vectorf Map_sound::get_pos() const
{
	return pos;
}

int Map_sound::get_default_volume() const
{
	return default_volume;
}
