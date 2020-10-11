#pragma once
#include <variant>
#include <queue>

#include "util.h"

struct Command
{
	enum class Command_type { JUMP, STOP_JUMP, MOVE, STOP_MOVE } type;
	std::variant<int> args;
};

class Controller
{
public:
	virtual Command pop_command() = 0;
	virtual bool command_available() = 0;
	virtual void update(float dt) = 0;
	virtual ~Controller() {}
};

class Player_controller : public Controller
{
	std::queue<Command> commands;

public:
	Command pop_command();
	bool command_available();
	void update(float dt);
};