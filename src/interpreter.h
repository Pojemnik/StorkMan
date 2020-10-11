#pragma once
#include "util.h"

enum class Command_code {
	NOTHING, CHANGE_RESOLUTION, MOVE_PLAYER, GET_POSITION, SET_MUSIC_VOLUME,
	SET_SOUND_VOLUME, SET_PLAYER_MAX_HP, HEAL_PLAYER, DEAL_DAMAGE, SCALE_HP_BAR,
	SET_PLAYER_MOVE_SPEED, SET_PLAYER_JUMP_FORCE, DRAW_PLAYERS_COLLISION,
	DRAW_CHUNKS_BORDERS
};

class Commands_interpreter
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
	static Command get_command(string s);
	static std::pair<Command_code, Vectorf> execute_command(Command cmd);
	static std::pair<Command_code, Vectorf> execute_command_raw(Command cmd);
	static Vectorf get_vectorf(const Command& cmd, std::string var_name);
	static Vectori get_vectori(const Command& cmd, std::string var_name);
	static float get_float(const Command& cmd, std::string var_name);
	static bool get_bool(const Command& cmd, std::string var_name,
		std::array<std::string, 2> true_false_string);
	static int get_int(const Command& cmd, std::string var_name);

	Commands_interpreter() {};

public:
	static std::pair<Command_code, Vectorf> get_and_execute_command(string s);
};
