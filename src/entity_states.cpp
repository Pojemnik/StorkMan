#include "entity_states.h"

void Run_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::MOVE);
	entity.move(entity.direction, 0);
	entity.send_message<int>(Message::Message_type::MOVED, static_cast<int>(entity.surface));
	std::cout << "Run enter" << std::endl;
}

std::pair<Entity_state*, Entity_stack_command> Run_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_stack_command::REPLACE);
	}
	if (!entity.on_ground)
	{
		return std::make_pair(new In_air_state(), Entity_stack_command::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
		switch (cmd.type)
		{
		case Command::Command_type::JUMP:
			return std::make_pair(new Run_jump_state(), Entity_stack_command::REPLACE);
			break;
		case Command::Command_type::MOVE:
			if (entity.direction != std::get<int>(cmd.args))
			{
				entity.direction = std::get<int>(cmd.args);
				return std::make_pair(new Run_state(), Entity_stack_command::REPLACE);
			}
			break;
		case Command::Command_type::STOP_MOVE:
			return std::make_pair(new Idle_state(), Entity_stack_command::REPLACE);
			break;
		case Command::Command_type::DOWN:
			entity.fallthrough = true;
			break;
		case Command::Command_type::STOP_DOWN:
			entity.fallthrough = false;
			break;
		default:
			break;
		}
	}
	entity.apply_force({ 0, context.gravity }, dt);
	entity.move(entity.direction, dt);
	return std::make_pair(nullptr, Entity_stack_command::NONE);
}

void Idle_jump_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::JUMP_IDLE);
	Jump_state::enter(entity);
	entity.send_message<bool>(Message::Message_type::JUMPED, false);
	std::cout << "Idle jump enter" << std::endl;
}

void Run_jump_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::JUMP_RUN);
	Jump_state::enter(entity);
	entity.send_message<bool>(Message::Message_type::JUMPED, true);
	std::cout << "Run jump enter" << std::endl;
}

void Jump_state::enter(Entity& entity)
{
	entity.jump(0);
	entity.on_ground = false;
}

std::pair<Entity_state*, Entity_stack_command> Jump_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_stack_command::REPLACE);
	}
	if (entity.on_ground)
	{
		//return std::make_pair(new Idle_state(), Entity_stack_command::REPLACE);
	}
	time_sum += dt;
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
		switch (cmd.type)
		{
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			entity.move(entity.direction, dt);
			break;
		case Command::Command_type::STOP_JUMP:
			return std::make_pair(new In_air_state(), Entity_stack_command::REPLACE);
		default:
			break;
		}
	}
	if (time_sum >= ASCENDING_TIME)
	{
		return std::make_pair(new In_air_state(), Entity_stack_command::REPLACE);
	}
	entity.jump(dt);
	return std::make_pair(nullptr, Entity_stack_command::NONE);
}

void Idle_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::IDLE);
	entity.send_message(Message::Message_type::STOPPED);
	std::cout << "Idle enter" << std::endl;
}

std::pair<Entity_state*, Entity_stack_command> Idle_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		return std::make_pair(new Die_state(), Entity_stack_command::REPLACE);
	}
	if (!entity.on_ground)
	{
		return std::make_pair(new In_air_state(), Entity_stack_command::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
		switch (cmd.type)
		{
		case Command::Command_type::JUMP:
			return std::make_pair(new Idle_jump_state(), Entity_stack_command::REPLACE);
			break;
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			return std::make_pair(new Run_state(), Entity_stack_command::REPLACE);
			break;
		case Command::Command_type::DOWN:
			entity.fallthrough = true;
			break;
		case Command::Command_type::STOP_DOWN:
			entity.fallthrough = false;
			break;
		default:
			break;
		}
	}
	entity.apply_force({ 0, context.gravity }, dt);
	assert(entity.get_current_animation() == Animation_index::IDLE);
	return std::make_pair(nullptr, Entity_stack_command::NONE);
}

void Die_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::DIE);
}

void Die_state::exit(Entity& entity)
{
	entity.send_message(Message::Message_type::DIED, NULL);
}

std::pair<Entity_state*, Entity_stack_command> Die_state::update(Entity& entity, float dt)
{
	(void)dt;
	while (entity.controller->command_available())
	{
		//Command being discarded
		(void)entity.controller->pop_command();
	}
	if (entity.get_current_animation() == Animation_index::DEFAULT)
	{
		return std::make_pair(nullptr, Entity_stack_command::POP);
	}
	assert(entity.get_current_animation() == Animation_index::DIE);
	return std::make_pair(nullptr, Entity_stack_command::NONE);
}

void In_air_state::enter(Entity& entity)
{
	if (entity.get_current_animation() == Animation_index::MOVE)
	{
		entity.set_animation(Animation_index::IDLE);
	}
	std::cout << "In air enter" << std::endl;
}

std::pair<Entity_state*, Entity_stack_command> In_air_state::update(Entity& entity, float dt)
{
	if (entity.health <= 0)
	{
		//This should be other animation
		return std::make_pair(new Die_state(), Entity_stack_command::REPLACE);
	}
	if (entity.on_ground)
	{
		return std::make_pair(new Idle_state(), Entity_stack_command::REPLACE);
	}
	while (entity.controller->command_available())
	{
		Command cmd = entity.controller->pop_command();
		switch (cmd.type)
		{
		case Command::Command_type::MOVE:
			entity.direction = std::get<int>(cmd.args);
			entity.move(entity.direction, dt);
			break;
		case Command::Command_type::DOWN:
			entity.fallthrough = true;
			break;
		case Command::Command_type::STOP_DOWN:
			entity.fallthrough = false;
			break;
		default:
			break;
		}
	}
	entity.apply_force({ 0, context.gravity }, dt);
	return std::make_pair(nullptr, Entity_stack_command::NONE);
}