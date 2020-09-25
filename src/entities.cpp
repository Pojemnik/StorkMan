#include "entities.h"

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
		//No break
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
		sprite.setOrigin({ info.character_position.x - info.part_size.x / 2,
			info.offset.y + info.part_size.y / 2 });
	}
	else
	{
		sprite.setOrigin({ info.character_position.x + info.part_size.x / 2,
			info.offset.y + info.part_size.y / 2 });
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