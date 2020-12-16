#pragma once
#include <stack>
#include "entity_state_core.h"

class Entity_state_machine
{
	std::stack<Entity_state*> state_stack;

public:
	void update(Entity& entity, float dt);
	bool empty() const;
	void push_state(Entity_state* state, Entity* entity);
	Entity_state_machine(Entity_state* state);
};
