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
	panels.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		panels.push_back(sf::Texture());
		if (!panels[i].loadFromFile("img/tex/PANELE0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error PANELE0" + std::to_string(i) + ".png" << std::endl;
		}
		panels[i].setRepeated(true);
	}
	metal.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		metal.push_back(sf::Texture());
		if (!metal[i].loadFromFile("img/tex/ZEBROWANIE0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error ZEBROWANIE0" + std::to_string(i) + ".png" << std::endl;
		}
		metal[i].setRepeated(true);
	}
	if (!asphalt.loadFromFile("img/tex/ASFALT.png"))
	{
		std::cout << "Error ASFALT.png" << std::endl;
	}
	asphalt.setRepeated(true);
	if (!concrete.loadFromFile("img/tex/BETON.png"))
	{
		std::cout << "Error BETON.png" << std::endl;
	}
	concrete.setRepeated(true);
	if (!ground.loadFromFile("img/tex/ZIEMIA.png"))
	{
		std::cout << "Error ZIEMIA.png" << std::endl;
	}
	ground.setRepeated(true);
	if (!grass.loadFromFile("img/tex/TRAWA.png"))
	{
		std::cout << "Error TRAWA.png" << std::endl;
	}
	grass.setRepeated(true);
	if (!rivet.loadFromFile("img/tex/NITY00.png"))
	{
		std::cout << "Error NITY00.png" << std::endl;
	}
	rivet.setRepeated(true);
	stork_run.reserve(80);
	sf::Image image;
	if (!image.loadFromFile("img/stork/run_ss_347_358_is_8_10.png"))
	{
		std::cout << "Stork run texture error" << std::endl;
	}
	for(int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			stork_run.push_back(sf::Texture());
			if (!stork_run[i*8+j].loadFromImage(image, sf::IntRect(j * 347, i * 358, 347, 358)))
			{
				std::cout << "Stork run texture error" << std::endl;
			}
		}
	}
}