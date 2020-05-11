#include <tinyxml2.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "assets.h"
#include "game.h"
#include "parser.h"
#include "util.h"
#include "console.h"

const std::string VERSION = "0.3.5";

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
		map.player->update(1);
		map.update(1);
		acc -= 1000.0f / context.fps;
		map.player->move_angled(move);
	}
	return updated;
}

void resize_window(Map& map, sf::RenderWindow& window, Assets& assets)
{

	assets.blurh.setUniform("blurSize", 1.0f / context.resolution.x);
	assets.blurv.setUniform("blurSize", 1.0f / context.resolution.y);
	map.calc_map_vertices();
	window.setSize(sf::Vector2u(context.resolution.x, context.resolution.y));
}

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	std::cout.sync_with_stdio(false);
	std::cout << "Stork'man version " + VERSION << std::endl;
	Assets assets;
	assets.load_assets();
	Parser parser(&assets);
	parser.parse_additional_textures("img/textures.txt");
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(context.resolution.x,
		context.resolution.y, desktop.bitsPerPixel),
		"StorkMan " + VERSION, sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	Map map;
	context.fps_counter.setFont(context.arial);
	context.fps_counter.setPosition(0, 0);
	std::string path = (argc == 2) ? argv[1] : "map/stork_map_example.xml";
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	if (error != tinyxml2::XMLError::XML_SUCCESS)
	{
		std::cerr << "Brak poziomu" << std::endl;
		return -1;
	}
	std::cout << "Initializing map..." << std::endl;
	tinyxml2::XMLElement* root = doc.FirstChildElement();
	map = parser.parse_map(root);
	map.background.setPosition(context.background_position);
	map.layer2.setPosition(context.layer2_position);
	std::cout << "done!" << std::endl;
	sf::FloatRect f(380, 70, 20, 60);//f(380, 55, 20, 70);
	Player player({ 400, 100 }, assets.pieces, assets.pieces_rect,
		assets.animations, f, assets.stork_tree, 1.92f,
		context.global_scale, 87.f);
	map.player = &player;
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
					std::pair<Command_code, Vectorf> code = 
						Console::get_and_execute_command();
					switch (code.first)
					{
					case Command_code::CHANGE_RESOLUTION:
						resize_window(map, window, assets);
						break;
					case Command_code::CHANGE_SCALE:
						map.rescale(context.global_scale);
						player.rescale(context.global_scale);
						break;
					case Command_code::MOVE_PLAYER:
						player.set_position(code.second);
						break;
					case Command_code::RELOAD_LIGHT:
						map.recalc_light();
						break;
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
		float time = (float)clock.getElapsedTime().asMicroseconds();
		time /= 1000.0f;
		if (time > 2500.0f / context.fps)
		{
			time = 2500.0f / context.fps;
		}
		acc += time;
		clock.restart();
		if (update(time, map, moved))
		{
			if (context.draw_fps_counter)
				context.fps_counter.setString(std::to_string(int(1000.f / acc)));
			acc = 0;
			window.clear();
			sf::Vector2f camera_pos = player.get_position();
			camera_pos -= sf::Vector2f((float)context.resolution.x / 2, (float)context.resolution.y / 2);
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
			context.final_states.transform = rs.transform;
			context.final_states.transform.translate(-map.level_size.x / 2, -map.level_size.y / 2);
			window.draw(map.light_sprite, context.final_states);
			if (context.draw_fps_counter)
				window.draw(context.fps_counter);
			if (context.draw_map_vertices)
			{
				sf::VertexArray tmp(sf::Lines, 2 * map.map_edges.size());
				for (size_t i = 0; i < map.map_edges.size(); i++)
				{
					tmp[2 * i] = sf::Vertex(map.map_edges[i].first, sf::Color(255, 255, 255, 255));
					tmp[2 * i + 1] = sf::Vertex(map.map_edges[i].second, sf::Color(255, 255, 255, 255));
				}
				window.draw(tmp, rs);
			}
			window.display();
		}
	}
	return 0;
}