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
		if (cmd.args.size() == 1)
		{
			try
			{
				context.fps = std::stof(cmd.args[0]);
				std::cout << "FPS set to " << context.fps << std::endl;
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
	else if (cmd.name == "gravity")
	{
		if (cmd.args.size() == 1)
		{
			try
			{
				context.gravity = std::stof(cmd.args[0]);
				std::cout << "Gravity set to " << context.gravity << std::endl;
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
	else if (cmd.name == "bgparallax")
	{
		if (cmd.args.size() == 1)
		{
			try
			{
				context.parrallax = std::stof(cmd.args[0]);
				std::cout << "Background parallax set to " << context.parrallax << std::endl;
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
	else if (cmd.name == "jumpforce")
	{
		if (cmd.args.size() == 1)
		{
			try
			{
				context.jump_force = std::stof(cmd.args[0]);
				std::cout << "Storkman jump force set to " << context.jump_force << std::endl;
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
	else if (cmd.name == "storkminspeed")
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
				context.min_move_speed = { tab[0],tab[1] };
				std::cout << "Stokman minimal speed set to " << context.min_move_speed.x << context.min_move_speed.y << std::endl;
			}

		}
		else
		{
			util::print_argument_number_error(2);
		}
	}
	else if (cmd.name == "storkmaxspeed")
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
				context.max_move_speed = { tab[0],tab[1] };
				std::cout << "Stokman maximal speed set to " << context.max_move_speed.x << context.max_move_speed.y << std::endl;
			}

		}
		else
		{
			util::print_argument_number_error(2);
		}
	}
	else if (cmd.name == "movespeedreduction")
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
				context.move_speed_reduction = { tab[0],tab[1] };
				std::cout << "Move speed reduction set to " << context.move_speed_reduction.x << context.move_speed_reduction.y << std::endl;
			}

		}
		else
		{
			util::print_argument_number_error(2);
		}
	}
	else if (cmd.name == "storkmovespeed")
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
					err = true;
					util::print_incorrect_argument_error(cmd.name, cmd.args[i]);
				}
			}
			if (!err)
			{
				context.player_move_speed = { tab[0],tab[1] };
				std::cout << "Stokman move speed set to " << context.player_move_speed.x << context.player_move_speed.y << std::endl;
			}

		}
		else
		{
			util::print_argument_number_error(2);
		}
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
