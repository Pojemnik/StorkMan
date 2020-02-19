#include <tinyxml2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "core.h"
#include "parser.h"

//Storkman ma 1,92m
float global_scale = 76.8f; //[px/m]

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	Assets assets;
	assets.loadAssets();
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 768, desktop.bitsPerPixel), "StorkMan", sf::Style::Titlebar | sf::Style::Close);
	const int FPS = 60;
	sf::Clock clock;
	Map map;
	if (argc == 2)	//Docelowo w tym miejscu powinien wczytywaæ tylko jeden level
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError tmp= doc.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root,std::make_shared<Assets>(assets));
	}
	Animatable a(Vectorf(200, 200), std::make_shared<std::vector<sf::Texture>>(assets.stork_run), 1.98f, global_scale);
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
			if (event.type == sf::Event::Resized)
			{
			}
		}
		window.clear();
		//Render
		window.draw(map);
		window.draw(a);
		a.next_frame();
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}