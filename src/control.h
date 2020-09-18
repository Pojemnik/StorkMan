#pragma once
#include "entities.h"
#include <queue>

class Player_controller : public Controller
{
	std::queue<Command> commands;

public:
	Command get_command();
	bool command_available();
	void update(float dt);
};