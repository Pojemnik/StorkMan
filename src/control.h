#pragma once
#include "entities.h"
#include <queue>

class Player_controller : public Controller
{
	std::queue<Command> commands;

public:
	Command pop_command();
	bool command_available();
	void update(float dt);
};