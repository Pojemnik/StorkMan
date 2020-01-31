#include <tinyxml2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "core.h"
#include "parser.h"

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	Assets assets;
	assets.loadAssets();
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktop.bitsPerPixel), "StorkMan");
	const int FPS = 60;
	sf::Clock clock;
	Level lvl = Level();
	Platform plat = Platform(Vectorf(100, 100), std::make_shared<sf::Texture>(assets.bricks[0]), { Vectorf(0,0), Vectorf(0,30), Vectorf(100,30), Vectorf(100,0) });
	lvl.addTexturable(plat);
	tinyxml2::XMLDocument map;	//Wydzieliæ do osobnej klasy
	if (argc == 2)	//Wczytywanie danych poziomu z pliku
	{
		map.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = map.FirstChildElement();
		Level lvl = parse_level(root, std::make_shared<Assets>(assets));
	}

	while (window.isOpen())
	{
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}
		}
		window.clear();
		//Render
		window.draw(lvl.texturables[0]);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}