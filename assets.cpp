#include "assets.h"

void Assets::loadAssets()
{
	sf::Texture t;
	if (!t.loadFromFile("img/tex/CEGLY00.PNG"))
	{
		std::cout << "Error" << std::endl;
	}
	bricks.push_back(std::shared_ptr<sf::Texture>(&t));
}