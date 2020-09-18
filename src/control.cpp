#include "control.h"

Command Player_controller::get_command()
{
	Command tmp = commands.front();
	commands.pop();
	return tmp;
}

bool Player_controller::command_available()
{
	return commands.size() > 0;
}

void Player_controller::update(float dt)
{
	if (window.hasFocus())//TODO: Trzeba sk¹dœ to braæ
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			//player.attack(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			//player.attack(2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				Command command;
				command.type = Command::Command_type::MOVE;
				command.args = 1;
				commands.push(command);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				Command command;
				command.type = Command::Command_type::MOVE;
				command.args = -1;
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
	}
}