#include "control.h"

Command Player_controller::pop_command()
{
	Command tmp = commands.front();
	commands.pop();
	return tmp;
}

bool Player_controller::command_available()
{
	return commands.size() > 0;
}

void Player_controller::update()
{
	if (!context.window_focus)
	{
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		//Attack1
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//Attack2
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
		(!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
	{
		Command command;
		command.type = Command::Command_type::STOP_MOVE;
		commands.push(command);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Command command;
			command.type = Command::Command_type::MOVE;
			command.args = -1;
			commands.push(command);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Command command;
			command.type = Command::Command_type::MOVE;
			command.args = 1;
			commands.push(command);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Command command;
		command.type = Command::Command_type::JUMP;
		commands.push(command);
	}
	else
	{
		Command command;
		command.type = Command::Command_type::STOP_JUMP;
		commands.push(command);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		Command command;
		command.type = Command::Command_type::DOWN;
		commands.push(command);
	}
	else
	{
		Command command;
		command.type = Command::Command_type::STOP_DOWN;
		commands.push(command);
	}
}

Command Idle_cotroller::pop_command()
{
	return Command(Command::Command_type::NONE, 0);
}

bool Idle_cotroller::command_available()
{
	return false;
}

void Idle_cotroller::update()
{}
