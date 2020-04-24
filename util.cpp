#include "util.h"

struct Context context;

Vectorf util::saturate(Vectorf val, const Vectorf max_val)
{
	if (val.x > max_val.x)
		val.x = max_val.x;
	if (val.x < -max_val.x)
		val.x = -max_val.x;
	if (val.y > max_val.y)
		val.y = max_val.y;
	if (val.y < -max_val.y)
		val.y = -max_val.y;
	return val;
}

float util::rdn(float s)
{
	return(s / 180 * PI);
}

float util::ang_reduce(float ang)
{
	ang = fmod(ang, 360.0f);
	if (ang < 0)
		ang += 360.0f;
	return ang;
}

void util::set_vectorf(Vectorf& vector, command& cmd, std::string var_name)
{
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
				util::print_incorrect_argument_error(cmd.name, cmd.args[i]);
			}
		}
		if (!err)
		{
			vector = { tab[0],tab[1] };
			std::cout << var_name + " set to " << vector.x << ' ' << vector.y << std::endl;
		}

	}
	else
	{
		util::print_argument_number_error(2);
	}
}

void util::set_float(float& var, command& cmd, std::string var_name)
{
	if (cmd.args.size() == 1)
	{
		try
		{
			var = std::stof(cmd.args[0]);
			std::cout << var_name + " set to " << var << std::endl;
		}
		catch (std::invalid_argument e)
		{
			util::print_incorrect_argument_error(cmd.name, cmd.args[0]);
		}
	}
	else
	{
		util::print_argument_number_error(1);
	}
}

void util::execute_command(util::command cmd)
{
	if (cmd.name == "col")
	{
		if (cmd.args.size() == 1)
		{
			if (cmd.args[0] == "1")
				context.draw_collisions = true;
			else
				context.draw_collisions = false;
			std::cout << "Player collision mesh " << ((context.draw_collisions) ? "drawn" : "hidden") << std::endl;
		}
		else
		{
			util::print_argument_number_error(1);
		}
	}
	else if (cmd.name == "fps")
	{
		set_float(context.fps, cmd, "FPS");
	}
	else if (cmd.name == "gravity")
	{
		set_float(context.gravity, cmd, "Gravity");
	}
	else if (cmd.name == "bgparallax")
	{
		set_float(context.parrallax, cmd, "Background parallax");
	}
	else if (cmd.name == "layer2parallax")
	{
		set_float(context.parrallax2, cmd, "Layer 2 parallax");
	}
	else if (cmd.name == "jumpforce")
	{
		set_float(context.jump_force, cmd, "Storkman jump force");
	}
	else if (cmd.name == "storkminspeed")
	{
		set_vectorf(context.min_move_speed, cmd, "Minimal storkman speed");
	}
	else if (cmd.name == "storkmaxspeed")
	{
		set_vectorf(context.max_move_speed, cmd, "Maximal storkman speed");
	}
	else if (cmd.name == "storkspeedreduction")
	{
		set_vectorf(context.move_speed_reduction, cmd, "Move speed reduction");
	}
	else if (cmd.name == "storkmovespeed")
	{
		set_vectorf(context.player_move_speed, cmd, "Move speed");
	}
	else if (cmd.name == "bgpos")
	{
		set_vectorf(context.background_position, cmd, "Background position");	
	}
	else if (cmd.name == "layer2pos")
	{
		set_vectorf(context.layer2_position, cmd, "Layer 2 position");
	}
	else if (cmd.name == "bgscale")
	{
		set_float(context.background_scale, cmd, "Background scale");
	}
	else if (cmd.name == "layer2scale")
	{
		set_float(context.layer2_scale, cmd, "Layer 2 scale");
	}
	else
		std::cerr << "Unknown command: " + cmd.name << std::endl;
}

util::command util::get_command()
{
	std::string input, tmp;
	util::command cmd;
	std::getline(std::cin, input);
	std::stringstream sstr(input);
	sstr >> cmd.name;
	while (sstr >> tmp)
	{
		cmd.args.push_back(tmp);
	}
	return cmd;
}

void util::print_argument_number_error(int correct_number)
{
	std::cerr << "Error. This command takes " << std::to_string(correct_number) << " argument(s)" << std::endl;
}

void util::print_incorrect_argument_error(std::string command, std::string what)
{
	std::cerr << command + ": " + "incorrect argument: " + what << std::endl;
}
