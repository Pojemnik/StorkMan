#include "assets.h"

void Assets::loadAssets()
{
	bricks.reserve(4);	//sf::Texture nie mo¿e byæ kopiowane
	for (int i = 0; i < 4; i++)
	{
		bricks.push_back(sf::Texture());
		if (!bricks[i].loadFromFile("img/tex/CEGLY0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error CEGLY0" + std::to_string(i) + ".png" << std::endl;
		}
		bricks[i].setRepeated(true);
	}
}