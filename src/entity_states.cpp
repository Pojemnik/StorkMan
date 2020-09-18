#include "entity_states.h"

void Idle_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::IDLE);
}

std::pair<Entity_state*, Entity_state_info> Idle_state::update(Entity& entity, float dt)
{
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->get_command();
		switch (cmd.type)
		{
		case Command::Command_type::JUMP:
			return std::make_pair(new Idle_jump_state(), Entity_state_info::REPLACE);
			break;
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			return std::make_pair(new Run_state(), Entity_state_info::REPLACE);
			break;
		default:
			break;
		}
	}
	assert(entity.get_animation() == Animation_index::IDLE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void Run_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::MOVE);
	entity.move(entity.direction);
}

std::pair<Entity_state*, Entity_state_info> Run_state::update(Entity& entity, float dt)
{

	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->get_command();
		switch (cmd.type)
		{
		case Command::Command_type::JUMP:
			return std::make_pair(new Run_jump_state(), Entity_state_info::REPLACE);
			break;
		case Command::Command_type::MOVE:
			if (entity.direction == std::get<int>(cmd.args))
				break;
			entity.direction = std::get<int>(cmd.args);
			return std::make_pair(new Run_state(), Entity_state_info::REPLACE);
			break;
		default:
			break;
		}
	}
	entity.move(entity.direction);
	assert(entity.get_animation() == Animation_index::MOVE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void Idle_jump_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::JUMP_IDLE);
	entity.jump();
}

std::pair<Entity_state*, Entity_state_info> Jump_state::update(Entity& entity, float dt)
{
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->get_command();
		switch (cmd.type)
		{
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			entity.move(entity.direction);
			break;
		case Command::Command_type::STOP_JUMP:
			return std::make_pair(new In_air_state(), Entity_state_info::REPLACE);
		default:
			break;
		}
	}
	return std::make_pair(nullptr, Entity_state_info::NONE);
}
