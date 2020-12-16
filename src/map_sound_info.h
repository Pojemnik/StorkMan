#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

struct Map_sound_info
{
	int id = -1;
	int sound = -1;
	bool relative = false;
	Vectorf pos;
	bool repeat = false;
	float attenuation = -1.f;
	float min_distance = -1.f;
	int volume = 0;
	float range = -1.f;
};
