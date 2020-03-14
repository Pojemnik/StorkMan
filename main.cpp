#include <tinyxml2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "game.h"
#include "parser.h"


//Storkman ma 1,92m
float global_scale = 51.2f; //[px/m]
float gravity = 2.f;

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	Assets assets;
	assets.load_assets();
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 576, desktop.bitsPerPixel), "StorkMan", sf::Style::Titlebar | sf::Style::Close);
	const int FPS = 60;
	sf::Clock clock;
	Map map;
	if (argc == 2)	//Docelowo w tym miejscu powinien wczytywaæ tylko jeden level
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError tmp = doc.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root, &assets);
	}
	const std::vector<const Animation*> v = { &assets.stork_idle, &assets.stork_run, &assets.stork_jump_idle, &assets.stork_jump_run };
	Player player({ 400, 100 }, v, 1.92f, global_scale, 87.f);
	map.player = &player;
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
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Tilde)
				{
					float a, b;
					std::cin >> a >> b;
					player.move({ a,b });
				}
				if (event.key.code == sf::Keyboard::G)
				{
					gravity = gravity == 0 ? 9.81 : 0;
				}
			}
			if (event.type == sf::Event::Resized)
			{
			}
		}
		if(player.status == Entity_status::MOVE)
			player.status = Entity_status::IDLE;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player.move({5, 0});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player.move({-5, 0 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (player.status == Entity_status::IDLE)
				player.status = Entity_status::JUMP_IDLE;
			if (player.status == Entity_status::MOVE)
				player.status = Entity_status::JUMP_RUN;
		}
		window.clear();
		//Animations
		player.next_frame();
		//Physics
		player.move({ 0, gravity });//Gravity test
		player.update();
		map.update();
		//Render
		sf::Vector2f camera_pos(0, 0);
		sf::RenderStates rs= sf::RenderStates::Default;
		rs.transform = sf::Transform().translate(-camera_pos);
		window.draw(map,rs);
		window.draw(player);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}