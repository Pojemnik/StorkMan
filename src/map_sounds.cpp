#include "map_sounds.h"

Map_sound::Map_sound(int sound_, Collision&& collision_, int volume_, int id_)
	: sound(sound_), collision(collision_), volume(volume_), id(id_)
{
	pos = Vectorf(collision.rect.left, collision.rect.top);
}

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
