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
	Map map;
	if (argc == 2)	//Wczytywanie danych poziomu z pliku
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError tmp= doc.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root,std::make_shared<Assets>(assets));
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
		window.draw(map);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}