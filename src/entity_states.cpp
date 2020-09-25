#include "entity_states.h"

void Run_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::MOVE);
	entity.move(entity.direction);
	entity.send_message(Message::Message_type::MOVED, static_cast<int>(entity.surface));
}

std::pair<Entity_state*, Entity_state_info> Run_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_state_info::REPLACE);
	}
	if (!entity.on_ground)
	{
		return std::make_pair(new In_air_state(), Entity_state_info::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
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
	entity.apply_force({ 0, context.gravity });
	assert(entity.get_current_animation() == Animation_index::MOVE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void Idle_jump_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::JUMP_IDLE);
	entity.jump();
	entity.send_message(Message::Message_type::JUMPED, NULL);
}

void Run_jump_state::enter(Entity& entity)
{
	//Problem
	//We need to know on which keyframe we are in order to make correct transition
	//On the other hand, it's not portable
	entity.set_animation(Animation_index::JUMP_RUN);
	entity.jump();
	entity.send_message(Message::Message_type::JUMPED, NULL);
}

std::pair<Entity_state*, Entity_state_info> Jump_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_state_info::REPLACE);
	}
	if (entity.on_ground)
	{
		return std::make_pair(new Idle_state(), Entity_state_info::REPLACE);
	}
	time_sum += dt;
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
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
	if (time_sum >= ASCENDING_TIME)
	{
		return std::make_pair(new In_air_state(), Entity_state_info::REPLACE);
	}
	entity.jump();
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void Idle_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::IDLE);
}

std::pair<Entity_state*, Entity_state_info> Idle_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		return std::make_pair(new Die_state(), Entity_state_info::REPLACE);
	}
	if (!entity.on_ground)
	{
		return std::make_pair(new In_air_state(), Entity_state_info::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
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
	entity.apply_force({ 0, context.gravity });
	assert(entity.get_current_animation() == Animation_index::IDLE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void Die_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::DIE);
}

void Die_state::exit(Entity& entity)
{
	entity.send_message(Message::Message_type::DIED, NULL);
}

std::pair<Entity_state*, Entity_state_info> Die_state::update(Entity& entity, float dt)
{
	while (entity.controller->command_available())
	{
		//Command being discarded
		(void)entity.controller->pop_command();
	}
	if (entity.get_current_animation().index == Animation_index::DEFAULT)
	{
		return std::make_pair(nullptr, Entity_state_info::POP);
	}
	assert(entity.get_current_animation() == Animation_index::DIE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void In_air_state::enter(Entity& entity)
{
}

std::pair<Entity_state*, Entity_state_info> In_air_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_state_info::REPLACE);
	}
	if (entity.on_ground)
	{
		return std::make_pair(new Idle_state(), Entity_state_info::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
		switch (cmd.type)
		{
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			entity.move(entity.direction);
			break;
		default:
			break;
		}
	}
	entity.apply_force({ 0, context.gravity });
	return std::make_pair(nullptr, Entity_state_info::NONE);
}
