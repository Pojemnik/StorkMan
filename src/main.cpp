#include <tinyxml2.h>
#include "parser.h"
#include "interpreter.h"
#include "ui.h"
#include "control.h"
#include "entity_states.h"

const std::string VERSION = "pre-alpha 0.4.9.1";

bool process_event(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		return true;
	case sf::Event::KeyPressed:
		if (context.window_focus)
		{
			if (event.key.code == sf::Keyboard::Tilde)
			{
				if (context.console->is_active())
				{
					context.console->deactivate();
				}
				else
				{
					context.console->activate(context.resolution);
				}
			}
			if (event.key.control && event.key.code == sf::Keyboard::V
				&& context.console->is_active())
			{
				string s = sf::Clipboard::getString();
				context.console->input_append(s);
			}
			if (event.key.code == sf::Keyboard::G &&
				!context.console->is_active())
			{
				context.gravity = -context.gravity;
			}
			if (event.key.code == sf::Keyboard::Up
				&& context.console->is_active())
			{
				context.console->get_next_history_line();
			}
			if (event.key.code == sf::Keyboard::Down
				&& context.console->is_active())
			{
				context.console->get_previous_history_line();
			}
		}
		break;
	case sf::Event::TextEntered:
		if (context.console->is_active() && context.window_focus)
		{
			if (event.text.unicode < 128)
			{
				char c = char(event.text.unicode);
				context.console->input_append(c);
			}
		}
		break;
	case sf::Event::MouseWheelScrolled:
		if (context.console->is_active() && context.window_focus)
		{
			context.console->scroll((int)event.mouseWheelScroll.delta);
		}
		break;
	}
	return false;
}

bool execute_init_file(string path)
{
	bool init = false;
	std::ifstream config("config.cfg");
	std::stringstream configsstream;
	string configstring;
	while (!config.eof())
	{
		std::getline(config, configstring);
		context.console->input_append(configstring + "\r\n");
		init = true;
	}
	return init;
}

int main(int argc, char** argv)
{
	//Assets
	Assets assets;
	assets.load_assets();

	//Console
	context.console = std::unique_ptr<Console>(
		new Console(assets.console_bg, &assets.consola, context.resolution));
	context.console->out << "Stork'man version " + VERSION << '\n';
	//Parsing
	Parser parser(&assets);

	//Window setup
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(context.resolution.x,
		context.resolution.y, desktop.bitsPerPixel),
		"StorkMan " + VERSION, sf::Style::Titlebar | sf::Style::Close);
	sf::Vector2u icon_size = assets.icon.getSize();
	window.setIcon(icon_size.x, icon_size.y, assets.icon.getPixelsPtr());

	//Map
	std::string path = (argc == 2) ? argv[1] : "map/map.xml";
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	if (error != tinyxml2::XMLError::XML_SUCCESS)
	{
		context.console->err << "Brak poziomu" << '\n';
		return -1;
	}
	context.console->log << "Initializing map..." << '\n';
	tinyxml2::XMLElement* root = doc.FirstChildElement();
	Map map = parser.parse_map(root);
	context.console->log << "done!" << '\n';

	//Player
	Entity_config storkman_config = parser.parse_entity_config("data/storkman.txt");
	auto animations = assets.load_dynamic_animations(storkman_config.animation_files);
	const auto stork_tree = assets.load_animation_tree(storkman_config.tree_file);
	assets.pieces = assets.load_entity_textures(storkman_config.texture_paths,
		storkman_config.textures_n, storkman_config.texture_size);
	std::vector<std::unique_ptr<Animation_part>> stork_parts;
	for (int i = 0; i < assets.pieces[0].size(); i++)
	{

		std::vector<const sf::Texture*> temp;
		temp.reserve(assets.pieces.size());
		for (int j = 0; j < assets.pieces.size(); j++)
		{
			temp.push_back(assets.pieces[j][i]);
		}
		stork_parts.push_back(std::make_unique<Multi_texture_animtion_part>(temp));
	}
	auto animation = std::make_unique<Key_frame_animation>(std::move(stork_parts),
		*animations, stork_tree);
	Physical physical(std::move(storkman_config.mesh), { 15 * context.global_scale,
		5 * context.global_scale });
	auto machine = std::make_unique<Entity_state_machine>(new Idle_state());
	auto controller = std::make_unique<Player_controller>();
	Entity player(std::move(animation), physical, std::move(machine),
		std::move(controller), storkman_config.height, storkman_config.max_hp,
		Entity_type::PLAYER);
	map.add_entity(&player);

	//Config file
	bool init = execute_init_file("config.cfg");

	//Sound init
	sf::Music music;
	music.openFromFile("sound/theme.wav");
	music.setLoop(true);
	music.play();

	//User interface
	sf::Text fps_counter;
	fps_counter.setFont(assets.consola);
	fps_counter.setPosition(0, 0);

	Hp_bar hp_bar(assets.hp_bar.bot, assets.hp_bar.mid, assets.hp_bar.top,
		assets.hp_bar.content_bot, assets.hp_bar.content_mid,
		assets.hp_bar.content_top, 1000);

	sf::Text hp;
	hp.setFont(assets.consola);
	hp.setPosition(100, 0);
	hp.setFillColor(sf::Color(200, 0, 0));
	hp.setString(std::to_string(player.health));

	//Other
	context.thread_pool = std::unique_ptr<ctpl::thread_pool>(new ctpl::thread_pool(4));
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		context.window_focus = window.hasFocus();
		while (window.pollEvent(event))
		{
			if (process_event(event))
			{
				window.close();
				return 0;
			}
		}
		if (context.console->is_active() || init)
		{
			if (init)
			{
				init = false;
			}
			while (context.console->user_input_data_available())
			{
				std::pair<Commands_interpreter::Command_code, Vectorf> code =
					Commands_interpreter::get_and_execute_command(
						context.console->get_user_input_line());
				switch (code.first)
				{
				case Commands_interpreter::Command_code::CHANGE_RESOLUTION:
					window.setSize(sf::Vector2u(context.resolution.x, context.resolution.y));
					break;
				case Commands_interpreter::Command_code::MOVE_PLAYER:
					player.set_position(code.second * context.global_scale);
					break;
				case Commands_interpreter::Command_code::GET_POSITION:
					context.console->out <<
						player.get_position() / context.global_scale << '\n';
					break;
				case Commands_interpreter::Command_code::SET_MUSIC_VOLUME:
					music.setVolume(code.second.x);
					break;
				case Commands_interpreter::Command_code::SET_SOUND_VOLUME:
					break;
				case Commands_interpreter::Command_code::SET_PLAYER_MAX_HP:
					player.set_max_health(int(code.second.x));
					hp_bar.set_max_hp(int(code.second.x));
					break;
				case Commands_interpreter::Command_code::HEAL_PLAYER:
					player.heal(player.get_max_health());
					break;
				case Commands_interpreter::Command_code::DEAL_DAMAGE:
					player.deal_damage(int(code.second.x));
					break;
				case Commands_interpreter::Command_code::SCALE_HP_BAR:
					hp_bar.scale_x(code.second.x);
					break;
				case Commands_interpreter::Command_code::SET_PLAYER_MOVE_SPEED:
					player.set_move_speed(code.second.x);
					break;
				case Commands_interpreter::Command_code::SET_PLAYER_JUMP_FORCE:
					player.set_jump_force(code.second.x);
					break;
				case Commands_interpreter::Command_code::DRAW_PLAYERS_COLLISION:
					player.set_draw_collision(static_cast<bool>(code.second.x));
					break;
				case Commands_interpreter::Command_code::DRAW_CHUNKS_BORDERS:
					map.set_draw_chunks_borders(static_cast<bool>(code.second.x));
					break;
				case Commands_interpreter::Command_code::SET_PLAYER_TEXTURE:
					player.set_textures_set(static_cast<int>(code.second.x));
					break;
				default:
					break;
				}
			}
		}
		if (context.console->output_available())
		{
			context.console->update_content();
		}
		float time = (float)clock.getElapsedTime().asMicroseconds();
		time /= 1000000.0f;
		time *= context.fps;
		clock.restart();
		Vectorf camera_pos = player.get_position();
		camera_pos -= Vectorf((float)context.default_resolution.x / 2,
			(float)context.default_resolution.y / 2);
		sf::FloatRect screen_rect(camera_pos.x, camera_pos.y,
			float(context.default_resolution.x), float(context.default_resolution.y));
		if (!context.console->is_active())
		{
			map.update(time, player.get_position(), screen_rect);
			player.update(time);
		}
		if (context.draw_fps_counter)
		{
			fps_counter.setString(std::to_string(int(context.fps / time)));
		}
		window.clear();
		camera_pos = player.get_position();
		camera_pos -= sf::Vector2f(context.default_resolution) / 2.0f;
		sf::RenderStates rs = sf::RenderStates::Default;
		rs.transform = sf::Transform().translate(-camera_pos);
		map.draw_bottom_layers(window, rs);
		window.draw(player, rs);
		map.draw_middle_layers(window, rs);
		map.draw_top_layers(window, rs);
		if (context.draw_damage_zones)
		{
			map.draw_zones(window, rs);
		}
		if (context.draw_map_vertices)
		{
			map.draw_vertices(window, rs);
		}
		if (context.draw_hp)
		{
			hp.setString(std::to_string(player.health));
			window.draw(hp);
		}
		hp_bar.pre_draw(player.health);
		window.draw(hp_bar);
		if (context.draw_fps_counter)
		{
			window.draw(fps_counter);
		}
		if (context.console->is_active())
		{
			window.draw(*context.console);
		}
		window.display();
	}
	return 0;
}