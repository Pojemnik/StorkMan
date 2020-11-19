#include "map_sounds.h"

Map_sound::Map_sound(const sf::SoundBuffer* buffer_, Collision&& collision_, int volume_)
	: buffer(buffer_), collision(collision_), volume(volume_)
{
}

Collision Map_sound::get_collision() const
{
	return collision;
}

const sf::SoundBuffer* Map_sound::get_buffer() const
{
	return buffer;
}
