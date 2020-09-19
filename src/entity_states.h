#pragma once
#include "entities.h"

struct Run_state : public Entity_state
{
	void enter(Entity& entity);
	std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt);
};

struct Jump_state : public Entity_state
{
	float time_sum = 0.f;
	const float ASCENDING_TIME = 10.f;
	virtual void enter(Entity& entity) = 0;
	virtual std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt);
};

struct Idle_jump_state : public Jump_state
{
	void enter(Entity& entity);
};

struct Run_jump_state : public Jump_state
{
	void enter(Entity& entity);
};

struct Idle_state : public Entity_state
{
	void enter(Entity& entity);
	std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt);
};

struct Die_state : public Entity_state
{
	void enter(Entity& entity);
	void exit(Entity& entity);
	std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt);
};

struct In_air_state : public Entity_state
{
	void exit(Entity& entity);
	std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt);
};
