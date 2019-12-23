#include <iostream>
#include <tinyxml2.h>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.h"

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	tinyxml2::XMLDocument map;
	if (argc == 2)	//Wczytywanie danych poziomu z pliku
	{
		map.LoadFile(argv[1]);
		tinyxml2::XMLNode* root = map.FirstChild();
		if (root == NULL)
		{
			std::cout << "B³¹d w pierwszym elemencie pliku!" << std::endl;
		}
	}
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktop.bitsPerPixel), "StorkMan");
	const int FPS = 60;
	sf::Clock clock;
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
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}