#pragma once
#include <SFML/Audio.hpp>
#include "collisions.h"

class Map_sound
{
	const sf::SoundBuffer* buffer;
	Collision collision;
	int volume;
	//Other sound params

public:
	Map_sound(const sf::SoundBuffer* buffer_, Collision&& collision_, int volume_);
	Collision get_collision() const;
	const sf::SoundBuffer* get_buffer() const;
};