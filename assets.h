#pragma once
#include "core.h"

struct Assets
{
	std::vector<std::shared_ptr<sf::Texture>> bricks;//Ju� nie rzuca b��dem, ale troch� temu nie ufam

	void loadAssets();
};