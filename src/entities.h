#pragma once
#include <stack>
#include <variant>
#include "physics.h"
#include "logic.h"
#include "animations.h"
#include "messaging.h"
#include "interfaces.h"

enum class Entity_state_info {PUSH, POP, REPLACE, NONE};

struct Entity_state
{
	virtual void enter(Entity& entity) = 0;
	virtual void exit(Entity& entity) {};
	virtual std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt) = 0;
};

class Entity_state_machine
{
	std::stack<Entity_state*> state_stack;

public:
	virtual void update(Entity& entity, float dt);
	Entity_state_machine(Entity_state* state);
};

struct Command
{
	enum class Command_type { JUMP, STOP_JUMP, MOVE } type;
	std::variant<int> args;
};

class Controller
{
public:
	virtual Command pop_command() = 0;
	virtual bool command_available() = 0;
	virtual void update(float dt) = 0;
};

class Entity : public Updatable, public Collidable, public Message_sender, public Renderable
{
protected:
	const float move_speed = 1.f;
	const float jump_force = 50.f;
	int last_direction = 1;
	Physical physical;
	sf::Sprite sprite;
	float height;
	std::unique_ptr<Animation> animation;
	std::unique_ptr<Entity_state_machine> state;
	int max_health;

	void flip(int direction);

public:
	std::unique_ptr<Controller> controller;
	int health;
	int last_dmgz_id = -1;
	int direction = 1;//x sign
	Vectorf collision_vector;
	Surface_type surface;
	bool on_ground;

	const Collision* const get_collision() const;
	void set_animation(Animation_index a);
	Animation_index get_current_animation();
	void move(int direction);
	void apply_force(Vectorf force);
	void jump();
	void deal_damage(int amount);
	void set_max_health(int val);
	int get_max_health();
	void heal(int amount);
	Vectorf get_position();
	void set_position(Vectorf new_position);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Entity(std::unique_ptr<Animation>&& animation_, Physical& physical_,
		std::unique_ptr<Entity_state_machine>&& state_,
		std::unique_ptr<Controller>&& controller_, float height_, int health_);
};