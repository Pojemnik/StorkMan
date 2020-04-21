#include <tinyxml2.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "game.h"
#include "parser.h"
#include "util.h"

const std::string VERSION = "0.3.3e";

bool update(float dt, Map& map, int move)
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
		float ang;
		switch (move)
		{
		case 0:
			break;
		case 1:

			if (map.player->maxcollisionvector.y == 0 && map.player->maxcollisionvector.x == 0)
				ang = 0;
			else
				ang = -atan2(map.player->maxcollisionvector.x, map.player->maxcollisionvector.y);
			map.player->move({ context.player_move_speed.x * cos(ang) - context.player_move_speed.y * sin(ang),context.player_move_speed.x * sin(ang) + context.player_move_speed.y * cos(ang) });
			break;
		case -1:
			if (map.player->maxcollisionvector.y == 0 && map.player->maxcollisionvector.x == 0)
				ang = 0;
			else
				ang = -atan2(map.player->maxcollisionvector.x, map.player->maxcollisionvector.y);
			map.player->move({ -context.player_move_speed.x * cos(ang) - context.player_move_speed.y * sin(ang),-context.player_move_speed.x * sin(ang) + context.player_move_speed.y * cos(ang) });
		}
	}
	return updated;
}

void resize_window(Map& map, sf::RenderWindow& window)
{

	context.blurh.setUniform("blurSize", 1.0f / context.resolution.x);
	context.blurv.setUniform("blurSize", 1.0f / context.resolution.y);
	context.lightmap.create(context.resolution.x, context.resolution.y);
	context.lm2.create(context.resolution.x / 2, context.resolution.y / 2);
	context.lm3.create(context.resolution.x / 2, context.resolution.y / 2);
	context.lm4.create(context.resolution.x, context.resolution.y);
	map.calc_map_vertices();
	window.setSize(sf::Vector2u(context.resolution.x, context.resolution.y));
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
	sf::RenderWindow window(sf::VideoMode(context.resolution.x, context.resolution.y, desktop.bitsPerPixel), "StorkMan " + VERSION, sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	std::cout << test->getElapsedTime().asMilliseconds() << std::endl;
	Map map;
	context.fps_counter.setFont(context.arial);
	context.fps_counter.setPosition(0, 0);
	if (argc == 2)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile(argv[1]);
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root, &assets);
	}
	else
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile("map/stork_map_example.xml");
		tinyxml2::XMLElement* root = doc.FirstChildElement();
		map = parse_map(root, &assets);
	}
	map.background.setPosition(context.background_position);
	map.layer2.setTexture(*assets.layer2);
	map.layer2.setPosition(context.layer2_position);
	sf::FloatRect f(380, 55, 20, 70);
	Player player({ 400, 100 }, assets.pieces, assets.pieces_rect, assets.animations, f, assets.stork_tree, 1.92f, global_scale, 87.f);
	map.player = &player;
	std::cout << test->getElapsedTime().asMilliseconds() << std::endl;
	int moved = 0;
	float acc = 0;
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
			if (event.type == sf::Event::KeyPressed && window.hasFocus())
			{
				if (event.key.code == sf::Keyboard::Tilde)
				{
					int flag = util::execute_command(util::get_command());//Da� tu enum
					switch (flag)
					{
					case 1:
						resize_window(map, window);
					default:
						break;
					}
				}
				if (event.key.code == sf::Keyboard::G)
				{
					context.gravity = -context.gravity;
				}
			}
		}
		moved = 0;
		if (window.hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				player.attack(1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.attack(2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					moved = 1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					moved = -1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					player.jump(true);
				else
					player.jump(false);
			}
		}
		float time = clock.getElapsedTime().asMicroseconds();
		time /= 1000.0f;
		if (time > 2500.0f / context.fps)
		{
			time = 2500.0f / context.fps;
		}
		acc += time;
		clock.restart();
		if (update(time, map, moved))
		{
			context.fps_counter.setString(std::to_string(int(1000.f / acc)));
			acc = 0;
			window.clear();
			sf::Vector2f camera_pos = player.get_position();
			camera_pos -= sf::Vector2f(context.resolution.x / 2, context.resolution.y / 2);
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
			std::vector<Vectorf> sources = { { 300, 400 }, {500,400}, {300,-1} };
			sf::Texture tex = map.calc_light(sources, rs.transform);
			sf::Sprite s(tex);
			window.draw(s, context.final_states);
			window.draw(context.fps_counter);
			if (context.draw_map_vertices)
			{
				sf::VertexArray tmp(sf::Lines, 2 * map.map_vertices.size());
				for (int i = 0; i < map.map_vertices.size(); i++)
				{
					tmp[2 * i] = sf::Vertex(map.map_vertices[i].first, sf::Color(255, 255, 255, 255));
					tmp[2 * i + 1] = sf::Vertex(map.map_vertices[i].second, sf::Color(255, 255, 255, 255));
				}
				window.draw(tmp, rs);
			}
			window.display();
		}
	}
	return 0;
}