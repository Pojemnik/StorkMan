#pragma once
#include <variant>
#include <queue>

#include "util.h"

struct Command
{
	enum class Command_type { JUMP, STOP_JUMP, MOVE, STOP_MOVE, DOWN, STOP_DOWN, NONE } type = Command_type::NONE;
	std::variant<int> args;

	Command() = default;
	template <typename T>
	Command(Command_type type_, T args_);
};

template <typename T>
Command::Command(Command_type type_, T args_) :
	type(type_), args(args_) {}


class Controller
{
public:
	virtual Command pop_command() = 0;
	virtual bool command_available() = 0;
	virtual void update() = 0;
	virtual ~Controller() {}
};

class Player_controller : public Controller
{
	std::queue<Command> commands;

public:
	Command pop_command();
	bool command_available();
	void update();
};

class Idle_cotroller : public Controller
{
	Command pop_command();
	bool command_available();
	void update();
};