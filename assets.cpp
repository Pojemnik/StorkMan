#include "assets.h"

void Assets::loadAssets()
{
	sf::Texture t;
	if (!t.loadFromFile("img/tex/CEGLY00.PNG"))
	{
		std::cout << "Error cegly00.png" << std::endl;
	}
	t.setRepeated(true);//Coœ tu jest nie tak(usuniêcie tej linijki nie zmienia efektu)
	bricks.push_back(t);
}