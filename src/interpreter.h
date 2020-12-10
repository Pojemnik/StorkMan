#pragma once
#include "util.h"

class Commands_interpreter : public Message_sender
{
public:
	enum class Command_code {
		NOTHING, CHANGE_RESOLUTION, MOVE_PLAYER, GET_POSITION,
		SET_PLAYER_MAX_HP, HEAL_PLAYER, DEAL_DAMAGE, SCALE_HP_BAR,
		SET_PLAYER_MOVE_SPEED, SET_PLAYER_JUMP_FORCE, DRAW_PLAYERS_COLLISION,
		DRAW_CHUNKS_BORDERS, SET_PLAYER_TEXTURE, DRAW_SOUND_SOURCES
	};

	struct Command
	{
		std::string name;
		std::vector<std::string> args;
	};
	std::pair<Command_code, Vectorf> get_and_execute_command(string s);
	Commands_interpreter();

private:
	std::string help_page;

	void load_help_page();
	void print_argument_number_error(int correct_number);
	void print_incorrect_argument_error(std::string command,
		std::string what);
	Command get_command(string s);
	std::pair<Command_code, Vectorf> execute_command(Command cmd);
	std::pair<Command_code, Vectorf> execute_command_raw(Command cmd);
	Vectorf get_vectorf(const Command& cmd, std::string var_name);
	Vectori get_vectori(const Command& cmd, std::string var_name);
	float get_float(const Command& cmd, std::string var_name);
	bool get_bool(const Command& cmd, std::string var_name,
		std::array<std::string, 2> true_false_string);
	int get_int(const Command& cmd, std::string var_name);
};
