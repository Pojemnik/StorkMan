#pragma once
#include "util.h"

enum class Command_code { NOTHING, CHANGE_RESOLUTION, CHANGE_SCALE,
	MOVE_PLAYER, RELOAD_LIGHT };

class Command_interpreter
{
	struct Command
	{
		std::string name;
		std::vector<std::string> args;
	};

private:
	static void print_argument_number_error(int correct_number);
	static void print_incorrect_argument_error(std::string command,
		std::string what);
	static Command get_command();
	static std::pair<Command_code, Vectorf> execute_command(Command cmd);
	static std::pair<Command_code, Vectorf> execute_command_raw(Command cmd);
	static Vectorf get_vectorf(const Command& cmd, std::string var_name);
	static Vectori get_vectori(const Command& cmd, std::string var_name);
	static float get_float(const Command& cmd, std::string var_name);
	static bool get_bool(const Command& cmd, std::string var_name,
		std::array<std::string, 2> true_false_string);
	static int get_int(const Command& cmd, std::string var_name);
	Command_interpreter() {};

public:
	static std::pair<Command_code, Vectorf> get_and_execute_command();
};

class Console : public sf::Drawable
{
public:

	Console(sf::Texture* tex, sf::Font* f, Vectori res);
	void activate(Vectori res);
	void deactivate();
	bool is_active();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool active = false;
	Vectori screen_resolution;
	sf::Sprite background;
	sf::Text content;
	sf::Font* font;
	sf::Text buffer;
};