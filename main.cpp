#include <tinyxml2.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "game.h"
#include "parser.h"
#include "util.h"

const std::string VERSION = "0.3.2";

bool update(float dt, Map& map)
{
	static float acc(0);
	acc += dt;
	bool updated = 0;
	if (acc >= 1000.0f / context.fps)
		updated = 1;
	while (acc >= 1000.0f / context.fps)
	{
		map.player->next_frame();
		map.player->apply_force({ 0, context.gravity });
		map.player->update(1);
		map.update(1);
		acc -= 1000.0f / context.fps;
	}
	return updated;
}

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	std::cout.sync_with_stdio(false);
	std::cout << "Stork'man version " + VERSION << std::endl;
	Assets assets;
	sf::Clock* test = new sf::Clock();
	test->restart();
	assets.load_assets();
	std::cout << test->getElapsedTime().asMilliseconds() << std::endl;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1024, 576, desktop.bitsPerPixel), "StorkMan " + VERSION, sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	std::cout << test->getElapsedTime().asMilliseconds() << std::endl;
	Map map;
	if (argc == 2)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root, &assets);
	}
	else
	{
		if (argc == 1)
		{
			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError error = doc.LoadFile("map/stork_map_example.xml");
			tinyxml2::XMLElement* root = doc.FirstChildElement();
			map = parse_map(root, &assets);
		}
	}
	map.background.setPosition(-1000, -2000);
	sf::FloatRect f(380, 55, 20, 70);
	Player player({ 400, 100 }, assets.pieces, assets.pieces_rect, assets.animations, f, assets.stork_tree, 1.92f, global_scale, 87.f);
	map.player = &player;
	std::cout << test->getElapsedTime().asMilliseconds() << std::endl;;
	while (window.isOpen())
	{
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
					util::execute_command(util::get_command());
				}
				if (event.key.code == sf::Keyboard::G)
				{
					context.gravity = -context.gravity;
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			player.attack(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.move(context.player_move_speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				player.move(-context.player_move_speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.jump(true);
			else
				player.jump(false);
		}

		float time = clock.getElapsedTime().asMicroseconds();
		time /= 1000;
		if (time > 2500.0f / context.fps)
		{
			time = 2500.0f / context.fps;
		}
		clock.restart();
		if (update(time, map))
		{
			window.clear();
			sf::Vector2f camera_pos = player.get_position();
			camera_pos -= sf::Vector2f(512, 288);
			sf::RenderStates rs = sf::RenderStates::Default;
			rs.transform = sf::Transform().translate(-camera_pos);
			window.draw(map, rs);
			if (context.draw_collisions)
			{
				sf::ConvexShape r = sf::ConvexShape(4);
				int i = 0;
				for (auto& it : player.mesh.vertices)
					r.setPoint(i, it), i++;
				r.setOutlineColor({ 255,0,0 });
				window.draw(r, rs);
			}
			window.draw(player, rs);
			window.display();
		}
	}
	return 0;
}