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
float gravity = .5f;

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
	const std::vector<const Animation*> v = { assets.stork_idle, assets.stork_run, assets.stork_jump_idle, assets.stork_jump_run };
	Player player({ 400, 100 }, v, 1.92f, global_scale, 87.f);
	map.player = &player;
	New_animatable a(assets.pieces, { 200,200 }, 1.92f, global_scale);
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
					float aa, bb;
					std::cin >> aa >> bb;
					player.move({ aa,-1*bb });
				}
				if (event.key.code == sf::Keyboard::G)
				{
					gravity = -gravity;
				}
			}
			if (event.type == sf::Event::Resized)
			{
			}
		}
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
			player.jump();
		}
		window.clear();
		//Animations
		player.next_frame();
		a.animate(192, 192, 0, 5, 5, 5, 0, -90, 0, 0, -90, 0, -100, 150, 60, -20, 40, 20, 80, 110, 20);
		//Physics
		player.apply_force({ 0, gravity });
		player.update();
		map.update();
		a.update();
		/*
		sf::ConvexShape r = sf::ConvexShape(4);
		int i = 0;
		for (auto& it : player.mesh_collision)
			r.setPoint(i,it), i++;
		r.setOutlineColor({255,255,255});
		*/
		//Render
		sf::Vector2f camera_pos=player.get_position();
		camera_pos-=sf::Vector2f(512,288);
		sf::RenderStates rs= sf::RenderStates::Default;
		rs.transform = sf::Transform().translate(-camera_pos);
		//window.draw(r,rs);
		window.draw(map,rs);
		window.draw(player,rs);
		window.draw(a);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 1000 / FPS);
	}
	return 0;
}