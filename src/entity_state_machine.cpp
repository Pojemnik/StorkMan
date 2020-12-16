#include "entity_state_machine.h"

void Entity_state_machine::update(Entity& entity, float dt)
{
	if (empty())
	{
		return;
	}
	std::pair<Entity_state*, Entity_stack_command> state = state_stack.top()->update(entity, dt);
	switch (state.second)
	{
	case Entity_stack_command::REPLACE:
		state_stack.top()->exit(entity);
		delete state_stack.top();
		state_stack.pop();
		[[fallthrough]];
	case Entity_stack_command::PUSH:
		state_stack.push(state.first);
		state_stack.top()->enter(entity);
		break;
	case Entity_stack_command::POP:
		state_stack.top()->exit(entity);
		delete state_stack.top();
		state_stack.pop();
		break;
	case Entity_stack_command::NONE:
		break;
	}
}

bool Entity_state_machine::empty() const
{
	return (state_stack.size() <= 0);
}

void Entity_state_machine::push_state(Entity_state* state, Entity* entity)
{
	state_stack.push(state);
	state_stack.top()->enter(*entity);
}

Entity_state_machine::Entity_state_machine(Entity_state* state)
{
	state_stack.push(state);
}