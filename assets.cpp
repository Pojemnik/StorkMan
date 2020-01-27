#include "assets.h"

void Assets::loadAssets()
{
	sf::Texture t;
	if (!t.loadFromFile("img/tex/CEGLY00.PNG"))
	{
		std::cout << "Error" << std::endl;
	}
	t.setRepeated(true);
	bricks.push_back(t);
}