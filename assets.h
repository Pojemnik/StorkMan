#pragma once
#include "core.h"

struct Assets
{
	std::vector<std::shared_ptr<sf::Texture>> bricks;//Ju¿ nie rzuca b³êdem, ale trochê temu nie ufam

	void loadAssets();
};