#pragma once
#include "core.h"

struct Assets
{
	std::vector<sf::Texture> bricks;
	std::vector<sf::Texture> panels;
	std::vector<sf::Texture> metal;
	std::vector<sf::Texture> stork_run;
	std::vector<sf::Texture> stork_idle;
	sf::Texture asphalt;
	sf::Texture concrete;
	sf::Texture ground;
	sf::Texture grass;
	sf::Texture rivet; //nit

	void loadAssets();
};