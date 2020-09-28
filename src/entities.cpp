#include "entities.h"

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
	entity.send_message(Message::Message_type::JUMPED, 0);
}

void Run_jump_state::enter(Entity& entity)
{
	entity.set_animation(Animation_index::JUMP_RUN);
	entity.jump();
	entity.send_message(Message::Message_type::JUMPED, 0);
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
	if (entity.get_current_animation() == Animation_index::DEFAULT)
	{
		return std::make_pair(nullptr, Entity_state_info::POP);
	}
	assert(entity.get_current_animation() == Animation_index::DIE);
	return std::make_pair(nullptr, Entity_state_info::NONE);
}

void In_air_state::enter(Entity& entity)
{
	(void)entity; //Unused
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

void Entity_state_machine::update(Entity& entity, float dt)
{
	if (state_stack.size() <= 0)
	{
		throw std::runtime_error("Out of entity stack!");
	}
	std::pair<Entity_state*, Entity_state_info> state = state_stack.top()->update(entity, dt);
	switch (state.second)
	{
	case Entity_state_info::REPLACE:
		state_stack.top()->exit(entity);
		delete state_stack.top();
		state_stack.pop();
		[[fallthrough]];
	case Entity_state_info::PUSH:
		state_stack.push(state.first);
		state_stack.top()->enter(entity);
		break;
	case Entity_state_info::POP:
		state_stack.top()->exit(entity);
		delete state_stack.top();
		state_stack.pop();
		break;
	case Entity_state_info::NONE:
		break;
	}
}

Entity_state_machine::Entity_state_machine(Entity_state* state)
{
	state_stack.push(state);
}

Entity::Entity(std::unique_ptr<Animation>&& animation_, Physical& physical_,
	std::unique_ptr<Entity_state_machine>&& state_,
	std::unique_ptr<Controller>&& controller_, float height_, int health_)
	: animation(std::move(animation_)), physical(physical_),
	state(std::move(state_)), controller(std::move(controller_)),
	health(health_), height(height_)
{
	set_max_health(health);
	Frame_info info = animation->get_frame_info();
	sprite.setOrigin({ info.character_position.x + info.part_size.x / 2,
			info.character_position.y + info.part_size.y / 2 });
}

const Collision* const Entity::get_collision() const
{
	return physical.get_collision();
}

void Entity::set_animation(Animation_index a)
{
	animation->set_animation(a);
}

Animation_index Entity::get_current_animation()
{
	return animation->get_current_animation();
}

void Entity::move(int direction)
{
	physical.move({ move_speed * direction, 0.f });
	if (last_direction != direction)
	{
		flip(direction);
		last_direction = direction;
	}
}

void Entity::apply_force(Vectorf force)
{
	physical.apply_force(force);
}

void Entity::jump()
{
	physical.apply_force({ 0, -jump_force });
}

void Entity::flip(int direction)
{
	Frame_info info = animation->get_frame_info();
	if (direction == -1)
	{
		sprite.setOrigin(Vectorf( info.character_position.x - info.part_size.x / 2,
			info.offset.y + info.part_size.y / 2 ));
	}
	else
	{
		sprite.setOrigin(Vectorf( info.character_position.x + info.part_size.x / 2,
			info.offset.y + info.part_size.y / 2 ));
	}
	sprite.scale(-1, 1);
}

void Entity::update(float dt)
{
	auto temp = physical.get_collision_info();
	collision_vector = temp.first;
	surface = temp.second;
	on_ground = physical.is_on_ground();
	state->update(*this, dt);
	physical.update(dt);
	animation->next_frame(dt);
	sprite.setTexture(*animation->get_texture());
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Vectorf Entity::get_position()
{
	return physical.get_pos();
}

void Entity::set_position(Vectorf new_position)
{
	physical.set_position(new_position);
}

void Entity::deal_damage(int amount)
{
	if (!context.god_mode)
	{
		health -= amount;
		if (health <= 0)
		{
			health = 0;
		}
	}
}

void Entity::set_max_health(int val)
{
	max_health = val;
	if (health > max_health)
		health = max_health;
}

int Entity::get_max_health()
{
	return max_health;
}

void Entity::heal(int amount)
{
	health += amount;
	if (health > max_health)
		health = max_health;
}