#include <tinyxml2.h>
#include "parser.h"
#include "interpreter.h"
#include "ui.h"
#include "control.h"
#include "entity_states.h"
#include "sound.h"
#include "edit_tools.h"

const std::string VERSION = "pre-alpha 0.5.2";

bool process_event(sf::Event& event, Message_sender& sender)
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
					context.console->activate();
				}
			}
			if (context.console->is_active())
			{
				if (event.key.control && event.key.code == sf::Keyboard::V)
				{
					string s = sf::Clipboard::getString();
					context.console->input_append(s);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					context.console->get_next_history_line();
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					context.console->get_previous_history_line();
				}
			}
			else
			{
				if (event.key.code == sf::Keyboard::G)
				{
					context.gravity = -context.gravity;
				}
				if (context.editor_mode)
				{
					if (event.key.code == sf::Keyboard::Delete)
					{
						if (!context.console->is_active())
						{
							sender.send_message(Message::Message_type::REMOVE_GRID_POINTS, NULL);
						}
					}
				}
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
	case sf::Event::MouseButtonPressed:
		if (context.window_focus && !context.console->is_active())
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (context.editor_mode)
				{
					sender.send_message(Message::Message_type::ADD_GRID_POINT, NULL);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (context.editor_mode)
				{
					sender.send_message(Message::Message_type::ADD_GRID_POINT, NULL);
				}
			}
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

	//Interpreter
	Commands_interpreter interpreter;

	//Parsing
	Parser parser(&assets);
	parser.load_music_config("sound/music/music.cfg");
	const auto sound_paths = parser.load_map_sound_config("sound/sound/map_sounds.cfg");

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
	map.add_receiver(&*context.console);

	//Player
	Entity_config storkman_config = parser.parse_entity_config("data/storkman.txt");
	assets.add_entity_sounds(storkman_config.type, storkman_config.sounds);
	auto animations = assets.load_dynamic_animations(storkman_config.animation_files);
	const auto stork_tree = assets.load_animation_tree(storkman_config.tree_file);
	assets.pieces = assets.load_entity_textures(storkman_config.texture_paths,
		storkman_config.textures_n, storkman_config.texture_size);
	std::vector<std::shared_ptr<Animation_part>> stork_parts;
	for (int i = 0; i < assets.pieces[0].size(); i++)
	{
		std::vector<const sf::Texture*> temp;
		temp.reserve(assets.pieces.size());
		for (int j = 0; j < assets.pieces.size(); j++)
		{
			temp.push_back(assets.pieces[j][i]);
		}
		stork_parts.push_back(std::make_shared<Multi_texture_animtion_part>(temp));
	}
	auto animation = std::make_unique<Key_frame_animation>(stork_parts,
		*animations, stork_tree);
	Physical physical(storkman_config.mesh, { 15 * context.global_scale,
		5 * context.global_scale });
	auto machine = std::make_unique<Entity_state_machine>(new Idle_state());
	std::unique_ptr<Controller> controller = std::make_unique<Player_controller>();
	Entity player(std::move(animation), physical, std::move(machine),
		std::move(controller), storkman_config.height, storkman_config.max_hp,
		Message_sender_type::PLAYER);
	player.add_receiver(context.console.get());
	map.add_entity(&player);

	//Test enemy
	animation = std::make_unique<Key_frame_animation>(stork_parts,
		*animations, stork_tree);
	Physical enemy_physical(storkman_config.mesh, { 20 * context.global_scale,
		5 * context.global_scale });
	machine = std::make_unique<Entity_state_machine>(new Idle_state());
	controller = std::make_unique<Idle_cotroller>();
	Entity test_enemy(std::move(animation), enemy_physical, std::move(machine),
		std::move(controller), storkman_config.height, storkman_config.max_hp,
		Message_sender_type::ENEMY);
	map.add_entity(&test_enemy);

	//Config file
	bool init = execute_init_file("config.cfg");

	//Sound init
	const auto steps_config = parser.load_steps_config("sound/sound/steps.cfg");
	Sound_system sound_system(assets.entity_sounds, parser.music_paths,
		sound_paths, steps_config);
	map.add_receiver(&sound_system);
	player.add_receiver(&sound_system);
	sound_system.add_receiver(&*context.console);
	interpreter.add_receiver(&sound_system);

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

	//Tools
	Grid grid(context.global_scale, 0.5, static_cast<Vectorf>(context.level_size),
		sf::Color(255, 255, 255, 50), sf::Color(0, 0, 0, 170), assets.consola);
	Tooltip tooltip({ 0, 0 }, assets.consola, sf::Color(0, 0, 0, 170));

	//Other
	context.thread_pool = std::unique_ptr<ctpl::thread_pool>(new ctpl::thread_pool(4));
	sf::Clock clock;
	map.init();
	Message_sender engine_sender(Message_sender_type::ENGINE);
	engine_sender.add_receiver(&sound_system);
	engine_sender.add_receiver(&*context.console);
	engine_sender.add_receiver(&grid);
	sf::SoundBuffer test_buffer;
	Vectorf camera_pos;
	camera_pos = player.get_position();
	camera_pos -= sf::Vector2f(context.resolution) / 2.0f;
	Vectorf mouse_pos = static_cast<Vectorf>(sf::Mouse::getPosition() -
		window.getPosition()) + camera_pos;
	mouse_pos += {-6.f, -31.f};
	while (window.isOpen())
	{
		sf::Event event;
		if (context.window_focus != window.hasFocus())
		{
			context.window_focus = window.hasFocus();
			engine_sender.send_message<bool>(Message::Message_type::WINDOW_FOCUS, context.window_focus);
		}
		while (window.pollEvent(event))
		{
			if (process_event(event, engine_sender))
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
					interpreter.get_and_execute_command(
						context.console->get_user_input_line());
				switch (code.first)
				{
				case Commands_interpreter::Command_code::CHANGE_RESOLUTION:
				{
					window.setSize(sf::Vector2u(context.resolution.x, context.resolution.y));
					sf::FloatRect visibleArea(0.f, 0.f, context.resolution.x, context.resolution.y);
					window.setView(sf::View(visibleArea));
					engine_sender.send_message<Vectori>(Message::Message_type::RESOLUTION_CHANGED, context.resolution);
				}
				break;
				case Commands_interpreter::Command_code::MOVE_PLAYER:
					player.set_position(code.second * context.global_scale);
					break;
				case Commands_interpreter::Command_code::GET_POSITION:
					context.console->out <<
						player.get_position() / context.global_scale << '\n';
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
				case Commands_interpreter::Command_code::DRAW_SOUND_SOURCES:
					map.set_draw_sound_sources(static_cast<bool>(code.second.x));
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

		//Update
		if (!context.console->is_active())
		{
			static float acc(0);
			const static float STEP(1);
			acc += time;
			sf::FloatRect screen_rect(camera_pos.x, camera_pos.y,
				float(context.resolution.x), float(context.resolution.y));
			while (acc > STEP)
			{
				map.update_physics(STEP, player.get_position(), screen_rect);
				test_enemy.update_physics(STEP);
				player.update_physics(STEP);
				context.player_pos = player.get_position();
				acc -= STEP;
			}
			map.update_graphics(time, player.get_position(), screen_rect);
			test_enemy.update_graphics(time);
			player.update_graphics(time);
			sound_system.update(time);
		}
		if (context.draw_fps_counter)
		{
			fps_counter.setString(std::to_string(int(context.fps / time)));
		}

		//Camera
		if (!context.editor_mode)
		{
			camera_pos = player.get_position();
			camera_pos -= sf::Vector2f(context.resolution) / 2.0f;
		}
		else
		{
			mouse_pos = static_cast<Vectorf>(sf::Mouse::getPosition() -
				window.getPosition()) + camera_pos;
			mouse_pos += {-6.f, -31.f}; //Magic numbers
			//mouse_pos = Vectorf(mouse_pos.x / context.camera_zoom.x,
			//	mouse_pos.y / context.camera_zoom.y);
		}
		sf::RenderStates rs = sf::RenderStates::Default;
		rs.transform = sf::Transform().translate(-camera_pos);

		//Drawing
		window.clear();
		map.draw_bottom_layers(window, rs);
		window.draw(player, rs);
		window.draw(test_enemy, rs);
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
		if (context.editor_mode)
		{
			grid.update(mouse_pos);
			window.draw(grid, rs);
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