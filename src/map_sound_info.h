#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

struct Map_sound_info
{
	int id;
	int sound;
	bool relative;
	Vectorf pos;
	bool repeat;
	float attenuation;
	float min_distance;
	float volume;
	float range;
};
