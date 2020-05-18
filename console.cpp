#include "util.h"
#include "console.h"

std::pair<Command_code, Vectorf>
Command_interpreter::get_and_execute_command(string s)
{
	return execute_command(get_command(s));
}

Console_stream::Console_stream(Stream_color c) : color(c){}

Console_stream& operator<<(Console_stream& stream, string& s)
{
	stream.buffer += s;
	return stream;
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
			*console << var_name + " set to " << vector.x << ' '
				<< vector.y << '\n';
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
			*console << var_name + " set to " << vector.x << ' '
				<< vector.y << '\n';
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
		*console << var_name + " " <<
			((val) ? true_false_string[0] : true_false_string[1]) << '\n';
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
			*console << var_name + " set to " << var << '\n';
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
			*console << var_name + " set to " << var << '\n';
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
			Vectorf(context.global_scale, 0));
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
		*console << "Unknown command: " + cmd.name << '\n';
	return std::make_pair<Command_code, Vectorf>(Command_code::NOTHING, { 0,0 });
}

Command_interpreter::Command Command_interpreter::get_command(string s)
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
	*console << "Error. This command takes " << std::to_string(correct_number) << " argument(s)" << '\n';
}

void Command_interpreter::print_incorrect_argument_error(std::string command, std::string what)
{
	*console << command + ": " + "incorrect argument: " + what << '\n';
}

Command_interpreter::Command_interpreter(Console* c) : console(c)
{
}

Console::Console(sf::Texture* tex, sf::Font* f, Vectori res) : font(f),
message(Stream_color::WHITE), log(Stream_color::GREY), error(Stream_color::RED)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setTexture(*tex);
	content.setFont(*f);
	content.setCharacterSize(15);
	buffer.setFont(*f);
	buffer.setCharacterSize(15);
	input_buffer = ">";
	buffer.setString(input_buffer + cursor);
}

void Console::activate(Vectori res)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setScale(scale);
	background.setPosition(0, res.y - 180);
	active = true;
	buffer.setPosition(0, screen_resolution.y - 20);
	update_content();
}

void Console::update_content()
{
	string content_string;
	int lines = 0;
	for (int i = content_history.size() - 1 - scroll_pos; i >= 0 && lines < lines_n; i--)
	{
		content_string = content_history[i] + '\n' + content_string;
		lines++;
	}
	content.setString(content_string);
	content.setPosition(0, screen_resolution.y - (lines+1) * 18);
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

void Console::input_append(char c)
{
	if (isprint(c) && c != '`')
	{
		input_buffer = input_buffer + c;
		buffer.setString(input_buffer + cursor);
	}
	else
	{
		if (c == '\r')
		{
			last_line = input_buffer.substr(1);
			content_history.push_back(input_buffer);
			input_buffer = '>';
			buffer.setString(input_buffer + cursor);
			update_content();
		}
		if (c == '\b')
		{
			if (input_buffer.length() > 1)
			{
				input_buffer.erase(input_buffer.length() - 1, 1);
				buffer.setString(input_buffer + cursor);
			}
		}
	}
}

void Console::print(string s)
{
	output_buffer += s;
}

void Console::print(char c)
{
	output_buffer += c;
	if (c == '\n')
		flush();
}

void Console::flush()
{
	size_t pos = output_buffer.find('\n');
	size_t last_pos = 0;
	while (pos != string::npos)
	{
		content_history.push_back(output_buffer.substr(last_pos, pos));
		last_pos = pos + 1;
		pos = output_buffer.find('\n', last_pos);
	}
	output_buffer = "";
	scroll_pos = 0;
	update_content();
}

void Console::scroll(int delta)
{
	scroll_pos += delta;
	scroll_pos = std::max(0, scroll_pos);
	scroll_pos = std::min(scroll_pos,
		std::max(0, (int)content_history.size() - lines_n));
	update_content();
}

string Console::get_line()
{
	return last_line;
}

inline Console& operator<<(Console& con, const string& obj)
{
	con.print(obj);
	return con;
}

inline Console& operator<<(Console& con, const int& obj)
{
	con.print(std::to_string(obj));
	return con;
}

inline Console& operator<<(Console& con, const float& obj)
{
	con.print(std::to_string(obj));
	return con;
}

inline Console& operator<<(Console& con, const char& obj)
{
	con.print(obj);
	return con;
}