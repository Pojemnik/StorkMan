#include "interpreter.h"

std::pair<Command_code, Vectorf>
Commands_interpreter::get_and_execute_command(string s)
{
	return execute_command(get_command(s));
}

Vectorf Commands_interpreter::get_vectorf(const Command& cmd, std::string var_name)
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
			context.console->out << var_name + " set to "
				<< (Vectorf)vector << '\n';
		}

	}
	else
	{
		print_argument_number_error(2);
		throw std::invalid_argument("Invalid argument");
	}
	return vector;
}

Vectori Commands_interpreter::get_vectori(const Command& cmd, std::string var_name)
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
			context.console->out << var_name + " set to " << (Vectorf)vector << '\n';
		}

	}
	else
	{
		print_argument_number_error(2);
		throw std::invalid_argument("Invalid argument");
	}
	return vector;
}

bool Commands_interpreter::get_bool(const Command& cmd, std::string var_name, std::array<std::string, 2> true_false_string)
{
	bool val = false;
	if (cmd.args.size() == 1)
	{
		if (cmd.args[0] == "1" || cmd.args[0] == "true" ||
			cmd.args[0] == "True" || cmd.args[0] == "treu")
			val = true;
		else
			val = false;
		context.console->out << var_name + " " <<
			((val) ? true_false_string[0] : true_false_string[1]) + '\n';
	}
	else
	{
		print_argument_number_error(1);
		throw std::invalid_argument("Invalid argument");
	}
	return val;
}

float Commands_interpreter::get_float(const Command& cmd, std::string var_name)
{
	float var = .0f;
	if (cmd.args.size() == 1)
	{
		try
		{
			var = std::stof(cmd.args[0]);
			context.console->out << var_name + " set to " << (float)var << '\n';
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

int Commands_interpreter::get_int(const Command& cmd, std::string var_name)
{
	int var = 0;
	if (cmd.args.size() == 1)
	{
		try
		{
			var = std::stoi(cmd.args[0]);
			context.console->out << var_name + " set to " << (int)var << '\n';
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

std::pair<Command_code, Vectorf> Commands_interpreter::execute_command_raw(Command cmd)
{
	if (cmd.name == "col")
	{
		bool col = get_bool(cmd, "Player collision mesh", { "drawn", "hidden" });
		return std::make_pair(Command_code::DRAW_PLAYERS_COLLISION,
			Vectorf(static_cast<int>(col), 0));
	}
	else if (cmd.name == "mapvertices")
	{
		context.draw_map_vertices =
			get_bool(cmd, "Map vertices", { "drawn", "hidden" });
	}
	else if (cmd.name == "drawhp")
	{
		context.draw_hp =
			get_bool(cmd, "Player's health level", { "drawn", "hidden" });
	}
	else if (cmd.name == "damagezones")
	{
		context.draw_damage_zones =
			get_bool(cmd, "Damage zones", { "drawn", "hidden" });
	}
	else if (cmd.name == "fpscounter")
	{
		context.draw_fps_counter =
			get_bool(cmd, "FPS counter", { "drawn", "hidden" });
	}
	else if (cmd.name == "godmode")
	{
		context.god_mode =
			get_bool(cmd, "God mode", { "on", "off" });
	}
	else if (cmd.name == "fps")
	{
		context.fps = get_float(cmd, "FPS");
	}
	else if (cmd.name == "gravity")
	{
		context.gravity = get_float(cmd, "Gravity");
	}
	else if (cmd.name == "jumpforce")
	{
		float force = get_float(cmd, "Storkman jump force");
		return std::make_pair(Command_code::SET_PLAYER_JUMP_FORCE, Vectorf(force, 0));
	}
	else if (cmd.name == "storkmovespeed")
	{
		float speed = get_float(cmd, "Move speed");
		return std::make_pair(Command_code::SET_PLAYER_MOVE_SPEED, Vectorf(speed, 0));
	}
	else if (cmd.name == "clear")
	{
		context.console->clear();
	}
	else if (cmd.name == "resolution")
	{
		context.resolution = get_vectori(cmd, "Resolution");
		return std::make_pair(Command_code::CHANGE_RESOLUTION,
			(Vectorf)context.resolution);
	}
	else if (cmd.name == "tp")
	{
		Vectorf target = get_vectorf(cmd, "Player moved");
		return std::make_pair(Command_code::MOVE_PLAYER, target);
	}
	else if (cmd.name == "getpos")
	{
		return std::make_pair(Command_code::GET_POSITION, Vectorf());
	}
	else if (cmd.name == "musicvolume")
	{
		float vol = get_float(cmd, "Music volume");
		if (vol < 0 || vol > 100)
		{
			throw std::invalid_argument("Incorrect argument");
		}
		return std::make_pair(Command_code::SET_MUSIC_VOLUME, Vectorf(vol, 0));
	}
	else if (cmd.name == "soundvolume")
	{
		float vol = get_float(cmd, "Sound volume");
		if (vol < 0 || vol > 100)
		{
			throw std::invalid_argument("Incorrect argument");
		}
		return std::make_pair(Command_code::SET_SOUND_VOLUME, Vectorf(vol, 0));
	}
	else if (cmd.name == "maxhealth")
	{
		int hp = get_int(cmd, "Player's max health");
		if (hp < 0)
		{
			throw std::invalid_argument("Incorrect argument");
		}
		return std::make_pair(Command_code::SET_PLAYER_MAX_HP, Vectorf(hp, 0));
	}
	else if (cmd.name == "heal")
	{
		context.console->out << "Player healed" << '\n';
		return std::make_pair(Command_code::HEAL_PLAYER, Vectorf());
	}
	else if (cmd.name == "dealdamage")
	{
		int dmg = get_int(cmd, "Damage value");
		if (dmg < 0)
		{
			throw std::invalid_argument("Incorrect argument");
		}
		return std::make_pair(Command_code::DEAL_DAMAGE, Vectorf(dmg, 0));
	}
	else if (cmd.name == "scalebar")
	{
		float val = get_float(cmd, "Hp bar scale");
		if (val < 0)
		{
			throw std::invalid_argument("Incorrect argument");
		}
		return std::make_pair(Command_code::SCALE_HP_BAR, Vectorf(val, 0));
	}
	else if (cmd.name == "chunksborders")
	{
		bool draw = get_bool(cmd, "Chunks' borders", { "drawn", "hidden" });
		return std::make_pair(Command_code::DRAW_CHUNKS_BORDERS,
			Vectorf(static_cast<bool>(draw), 0));
	}
	else
		context.console->err << "Unknown command: " + cmd.name << '\n';
	return std::make_pair<Command_code, Vectorf>(Command_code::NOTHING, Vectorf());
}

Commands_interpreter::Command Commands_interpreter::get_command(string s)
{
	std::string tmp;
	Command cmd;
	std::stringstream sstr(s);
	sstr >> cmd.name;
	while (sstr >> tmp)
	{
		cmd.args.push_back(tmp);
	}
	return cmd;
}

std::pair<Command_code, Vectorf> Commands_interpreter::execute_command(Command cmd)
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

void Commands_interpreter::print_argument_number_error(int correct_number)
{
	context.console->err << "Error. This command takes " << std::to_string(correct_number) << " argument(s)" << '\n';
}

void Commands_interpreter::print_incorrect_argument_error(std::string command, std::string what)
{
	context.console->err << command + ": " + "incorrect argument: " + what << '\n';
}