#include "assets.h"

void Assets::loadAssets()
{
	sf::Texture t;
	if (!t.loadFromFile("img/tex/CEGLY00.PNG"))
	{
		std::cout << "Error cegly00.png" << std::endl;
	}
	t.setRepeated(true);//Co� tu jest nie tak(usuni�cie tej linijki nie zmienia efektu)
	bricks.push_back(t);
}