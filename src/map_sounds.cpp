#include "map_sounds.h"

Map_sound::Map_sound(std::vector<Vectorf>&& mesh, Map_sound_info info_)
	: collision(mesh, info_.pos), info(info_) {}

Collision Map_sound::get_collision() const
{
	return collision;
}

int Map_sound::get_sound() const
{
	return info.sound;
}

int Map_sound::get_id() const
{
	return info.id;
}

Vectorf Map_sound::get_pos() const
{
	return info.pos;
}

int Map_sound::get_default_volume() const
{
	return info.volume;
}

Map_sound_info Map_sound::get_info() const
{
	return info;
}
