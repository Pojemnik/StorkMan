#include <tinyxml2.h>
#include "parser.h"
#include "interpreter.h"
#include "ui.h"
#include "control.h"

const std::string VERSION = "pre-alpha 0.4.6.5";

bool update(float dt, Map& map, Entity& player, sf::FloatRect screen)
{
	static float acc(0);
	acc += dt;
	bool updated = false;
	if (acc >= 1000.0f / context.fps)
		updated = true;
	while (acc >= 1000.0f / context.fps)
	{
		player.update(1);
		map.update(1, player.get_position(), screen);
		acc -= 1000.0f / context.fps;
	}
	return updated;
}

void resize_window(sf::RenderWindow& window, Assets& assets)
{
	assets.blurh.setUniform("blurSize", 1.0f / context.resolution.x);
	assets.blurv.setUniform("blurSize", 1.0f / context.resolution.y);
	window.setSize(sf::Vector2u(context.resolution.x, context.resolution.y));
}

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
	case sf::Event::KeyReleased:
		if (context.window_focus)
		{
			if (event.key.code == sf::Keyboard::Up
				&& !context.console->is_active())
			{
				context.jump_available = true;
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

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	//Assets
	Assets assets;
	assets.load_assets();
	context.console =
		new Console(assets.console_bg, &assets.consola, context.resolution);
	context.console->out << "Stork'man version " + VERSION << '\n';

	//Parsing
	Parser parser(&assets);
	parser.parse_additional_textures("img/textures.txt");
	parser.parse_additional_animations("img/animations.txt");

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
	auto animation = std::make_unique<Dynamic_animation>(assets.pieces, assets.pieces_rect,
		assets.dynamic_animations, assets.stork_tree);
	std::vector<Vectorf> mesh = { {20, -30}, {40, -30}, {40, 30}, {20, 30} };
	Physical physical(std::move(mesh), { 400, 100 });
	auto machine = std::make_unique<Entity_state_machine>(new Idle_state());
	auto controller = std::make_unique<Player_controller>();
	Entity player(std::move(animation), physical, std::move(machine),
		std::move(controller), 1.92f, 1000);

	//Config file
	bool init = execute_init_file("config.cfg");

	//Sound init
	sf::Music music;
	music.openFromFile("sound/theme.wav");
	music.setLoop(true);
	music.play();

	//User interface
	context.fps_counter.setFont(assets.consola);
	context.fps_counter.setPosition(0, 0);

	Hp_bar hp_bar(assets.hp_bar.bot, assets.hp_bar.mid, assets.hp_bar.top,
		assets.hp_bar.content_bot, assets.hp_bar.content_mid,
		assets.hp_bar.content_top, 1000);

	sf::Text hp;
	hp.setFont(assets.consola);
	hp.setPosition(100, 0);
	hp.setFillColor(sf::Color(200, 0, 0));
	hp.setString(std::to_string(player.health));

	//Other
	context.thread_pool = new ctpl::thread_pool(4);
	sf::Clock clock;
	float acc = 0;

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
				std::pair<Command_code, Vectorf> code =
					Commands_interpreter::get_and_execute_command(
						context.console->get_user_input_line());
				switch (code.first)
				{
				case Command_code::CHANGE_RESOLUTION:
					resize_window(window, assets);
					break;
				//case Command_code::CHANGE_SCALE:
				//	map.rescale(context.global_scale);
				//	player.rescale(context.global_scale);
				//	break;
				case Command_code::MOVE_PLAYER:
					player.set_position(code.second * context.global_scale);
					break;
				//case Command_code::RELOAD_LIGHT:
				//	map.recalc_light();
				//	break;
				case Command_code::GET_POSITION:
					context.console->out <<
						player.get_position() / context.global_scale << '\n';
					break;
				case Command_code::SET_MUSIC_VOLUME:
					music.setVolume(code.second.x);
					break;
				case Command_code::SET_SOUND_VOLUME:
					context.aaa.setVolume(code.second.x);
					context.jump_idle.setVolume(code.second.x);
					context.jump_run.setVolume(code.second.x);
					break;
				case Command_code::SET_PLAYER_MAX_HP:
					player.set_max_health(code.second.x);
					hp_bar.set_max_hp(code.second.x);
					break;
				case Command_code::HEAL_PLAYER:
					player.heal(player.get_max_health());
					break;
				case Command_code::DEAL_DAMAGE:
					player.deal_damage(code.second.x);
					break;
				case Command_code::SCALE_HP_BAR:
					hp_bar.scale_x(code.second.x);
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
		time /= 1000.0f;
		if (time > 2500.0f / context.fps)
		{
			time = 2500.0f / context.fps;
		}
		acc += time;
		clock.restart();
		Vectorf camera_pos = player.get_position();
		camera_pos -= Vectorf((float)context.default_resolution.x / 2,
			(float)context.default_resolution.y / 2);
		sf::FloatRect screen_rect(camera_pos.x, camera_pos.y,
		context.default_resolution.x, context.default_resolution.y);
		if (context.console->is_active() || update(time, map, player, screen_rect))
		{
			if (context.draw_fps_counter)
				context.fps_counter.setString(std::to_string(int(1000.f / acc)));
			acc = 0;
			window.clear();
			camera_pos = player.get_position();
			camera_pos -= sf::Vector2f((float)context.default_resolution.x / 2,
				(float)context.default_resolution.y / 2);
			sf::RenderStates rs = sf::RenderStates::Default;
			rs.transform = sf::Transform().translate(-camera_pos);
			map.draw_bottom_layers(window, rs);
			window.draw(player, rs);
			map.draw_middle_layers(window, rs);
			map.draw_top_layers(window, rs);
			if (context.draw_hp)
			{
				hp.setString(std::to_string(player.health));
				window.draw(hp);
			}
			hp_bar.pre_draw(player.health);
			window.draw(hp_bar);
			if (context.draw_fps_counter)
				window.draw(context.fps_counter);
			if (context.console->is_active())
			{
				window.draw(*context.console);
			}
			window.display();
		}
	}
	return 0;
}