#include "util.h"
#include "console.h"

std::pair<Command_code, Vectorf> Command_interpreter::get_and_execute_command()
{
	return execute_command(get_command());
}

Vectorf Command_interpreter::get_vectorf(const Command& cmd, std::string var_name)
{
	Vectorf vector = { 0,0 };
	if (cmd.args.size() == 2)
	{
		float tab[2];
		bool err = 0;
		for (int i = 0; i < 2; i++)
		{
			try
			{
				tab[i] = std::stof(cmd.args[i]);
			}
			catch (std::invalid_argument e)
			{
				err = 1;
				print_incorrect_argument_error(cmd.name, cmd.args[i]);
				throw std::invalid_argument("Invalid argument");
			}
		}
		if (!err)
		{
			vector = { tab[0],tab[1] };
			std::cout << var_name + " set to " << vector.x << ' '
				<< vector.y << std::endl;
		}

	}
	else
	{
		print_argument_number_error(2);
		throw std::invalid_argument("Invalid argument");
	}
	return vector;
}

Vectori Command_interpreter::get_vectori(const Command& cmd, std::string var_name)
{
	Vectori vector = { 0,0 };
	if (cmd.args.size() == 2)
	{
		int tab[2];
		bool err = 0;
		for (int i = 0; i < 2; i++)
		{
			try
			{
				tab[i] = std::stoi(cmd.args[i]);
			}
			catch (std::invalid_argument e)
			{
				err = 1;
				print_incorrect_argument_error(cmd.name, cmd.args[i]);
				throw std::invalid_argument("Invalid argument");
			}
		}
		if (!err)
		{
			vector = { tab[0],tab[1] };
			std::cout << var_name + " set to " << vector.x << ' '
				<< vector.y << std::endl;
		}

	}
	else
	{
		print_argument_number_error(2);
		throw std::invalid_argument("Invalid argument");
	}
	return vector;
}

bool Command_interpreter::get_bool(const Command& cmd, std::string var_name, std::array<std::string, 2> true_false_string)
{
	bool val = false;
	if (cmd.args.size() == 1)
	{
		if (cmd.args[0] == "1" || cmd.args[0] == "true" ||
			cmd.args[0] == "True" || cmd.args[0] == "treu")
			val = true;
		else
			val = false;
		std::cout << var_name + " " << 
			((val) ? true_false_string[0] : true_false_string[1]) << std::endl;
	}
	else
	{
		print_argument_number_error(1);
		throw std::invalid_argument("Invalid argument");
	}
	return val;
}

float Command_interpreter::get_float(const Command& cmd, std::string var_name)
{
	float var = .0f;
	if (cmd.args.size() == 1)
	{
		try
		{
			var = std::stof(cmd.args[0]);
			std::cout << var_name + " set to " << var << std::endl;
		}
		catch (std::invalid_argument e)
		{
			print_incorrect_argument_error(cmd.name, cmd.args[0]);
			throw std::invalid_argument("Invalid argument");
		}
	}
	else
	{
		print_argument_number_error(1);
		throw std::invalid_argument("Invalid argument");
	}
	return var;
}

int Command_interpreter::get_int(const Command& cmd, std::string var_name)
{
	int var = 0;
	if (cmd.args.size() == 1)
	{
		try
		{
			var = std::stoi(cmd.args[0]);
			std::cout << var_name + " set to " << var << std::endl;
		}
		catch (std::invalid_argument e)
		{
			print_incorrect_argument_error(cmd.name, cmd.args[0]);
			throw std::invalid_argument("Invalid argument");
		}
	}
	else
	{
		print_argument_number_error(1);
		throw std::invalid_argument("Invalid argument");
	}
	return var;
}

std::pair<Command_code, Vectorf> Command_interpreter::execute_command_raw(Command cmd)
{
	if (cmd.name == "col")
	{
		context.draw_collisions = 
			get_bool(cmd, "Player collision mesh", { "drawn", "hidden" });
	}
	else if (cmd.name == "mapvertices")
	{
		context.draw_map_vertices = 
			get_bool(cmd, "Map vertices", { "drawn", "hidden" });
	}
	else if (cmd.name == "fpscounter")
	{
		context.draw_fps_counter = 
			get_bool(cmd, "FPS counter", { "drawn", "hidden" });
	}
	else if (cmd.name == "night")
	{
		context.night = get_bool(cmd, "Night", { "on", "off" });
	}
	else if (cmd.name == "fps")
	{
		context.fps = get_float(cmd, "FPS");
	}
	else if (cmd.name == "gravity")
	{
		context.gravity = get_float(cmd, "Gravity");
	}
	else if (cmd.name == "bgparallax")
	{
		context.parallax = get_float(cmd, "Background parallax");
	}
	else if (cmd.name == "layer2parallax")
	{
		context.parallax2 = get_float(cmd, "Layer 2 parallax");
	}
	else if (cmd.name == "jumpforce")
	{
		context.jump_force = get_float(cmd, "Storkman jump force");
	}
	else if (cmd.name == "storkminspeed")
	{
		context.min_move_speed = get_float(cmd, "Minimal storkman speed");
	}
	else if (cmd.name == "storkmaxspeed")
	{
		context.max_move_speed = get_vectorf(cmd, "Maximal storkman speed");
	}
	else if (cmd.name == "storkspeedreduction")
	{
		context.move_speed_reduction = get_vectorf(cmd, "Move speed reduction");
	}
	else if (cmd.name == "storkmovespeed")
	{
		context.player_move_speed = get_vectorf(cmd, "Move speed");
	}
	else if (cmd.name == "bgpos")
	{
		context.background_position = get_vectorf(cmd, "Background position");
	}
	else if (cmd.name == "layer2pos")
	{
		context.layer2_position = get_vectorf(cmd, "Layer 2 position");
	}
	else if (cmd.name == "bgscale")
	{
		context.background_scale = get_float(cmd, "Background scale");
	}
	else if (cmd.name == "layer2scale")
	{
		context.layer2_scale = get_float(cmd, "Layer 2 scale");
	}
	else if (cmd.name == "darkness")
	{
		context.darkness = get_int(cmd, "Darkness level");
	}
	else if (cmd.name == "resolution")
	{
		context.resolution = get_vectori(cmd, "Resolution");
		return std::make_pair(Command_code::CHANGE_RESOLUTION,
			(Vectorf)context.resolution);
	}
	else if (cmd.name == "scale")
	{
		context.global_scale = get_float(cmd, "Global scale");
		return std::make_pair(Command_code::CHANGE_SCALE,
			Vectorf(context.global_scale,0));
	}
	else if (cmd.name == "moveplayer")
	{
		Vectorf target = get_vectorf(cmd, "Player moved");
		return std::make_pair(Command_code::MOVE_PLAYER, (Vectorf)target);
	}
	else if (cmd.name == "reloadlight")
	{
		return std::make_pair(Command_code::RELOAD_LIGHT, Vectorf(0, 0));
	}
	else
		std::cerr << "Unknown command: " + cmd.name << std::endl;
	return std::make_pair<Command_code, Vectorf>(Command_code::NOTHING, { 0,0 });
}

Command_interpreter::Command Command_interpreter::get_command()
{
	std::string input, tmp;
	Command cmd;
	std::getline(std::cin, input);
	std::stringstream sstr(input);
	sstr >> cmd.name;
	while (sstr >> tmp)
	{
		cmd.args.push_back(tmp);
	}
	return cmd;
}

std::pair<Command_code, Vectorf> Command_interpreter::execute_command(Command cmd)
{
	try
	{
		return execute_command_raw(cmd);
	}
	catch (std::invalid_argument e)
	{
		return std::make_pair<Command_code, Vectorf>(Command_code::NOTHING, { 0,0 });
	}
}

void Command_interpreter::print_argument_number_error(int correct_number)
{
	std::cerr << "Error. This command takes " << std::to_string(correct_number) << " argument(s)" << std::endl;
}

void Command_interpreter::print_incorrect_argument_error(std::string command, std::string what)
{
	std::cerr << command + ": " + "incorrect argument: " + what << std::endl;
}

Console::Console(sf::Texture* tex, sf::Font* f, Vectori res) : font(f)
{
	screen_resolution = res;
	float scale = (float)res.x / 1920.f;
	background.setTexture(*tex);
	background.setScale(scale, scale);
	background.setPosition(0, res.y / 2);
	content.setFont(*f);
	buffer.setFont(*f);
}

void Console::activate(Vectori res)
{
	screen_resolution = res;
	float scale = (float)res.x / 1920.f;
	background.setScale(scale, scale);
	background.setPosition(0,res.y/2);
	active = true;
}

void Console::deactivate()
{
	active = false;
}

bool Console::is_active()
{
	return active;
}

void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(content);
	target.draw(buffer);
}
