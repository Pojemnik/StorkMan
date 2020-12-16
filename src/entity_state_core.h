#pragma once
#include <utility>

enum class Entity_stack_command { PUSH, POP, REPLACE, NONE };

class Entity;

struct Entity_state
{
	virtual void enter(Entity& entity) = 0;
	virtual void exit(Entity& entity) { (void)entity; /*Unused*/ };
	virtual std::pair<Entity_state*, Entity_stack_command> update(Entity& entity, float dt) = 0;
	virtual ~Entity_state() {};
};
