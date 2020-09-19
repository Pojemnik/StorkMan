#pragma once
#include <stack>
#include <variant>
#include "physics.h"
#include "logic.h"
#include "animations.h"
#include "interfaces.h"

enum class Entity_state_info {PUSH, POP, REPLACE, NONE};

struct Entity_state
{
	virtual void enter(Entity& entity) = 0;
	virtual void exit(Entity& entity) {};
	virtual std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt) = 0;
};

struct Command
{
	enum class Command_type { JUMP, STOP_JUMP, MOVE } type;
	std::variant<int> args;
};

class Controller : public Updatable
{
public:
	virtual Command pop_command() = 0;
	virtual bool command_available() = 0;
};

class Entity : public Animatable, public Updatable, public Collidable
{
protected:
	const float move_speed = 1.f;
	const float jump_force = 50.f;
	Physical physical;
	sf::Sprite sprite;
	float height;
	std::unique_ptr<Animation> animation;
	std::stack<Entity_state*> state_stack;
	
	int max_health;

	void update_position(float dt);
	void set_idle();
	void flip_if_needed();
	void edge_jump_update();
	void die();

public:
	std::unique_ptr<Controller> controller;
	int health;
	int last_dmgz_id = -1;
	int direction = 1;//x sign

	void set_animation(Animation_index a);
	Animation_index get_animation_info();
	void move(int direction);
	void jump();
	//---------------------------------------------------------
	Entity(Vectorf p, std::unique_ptr<Animation>&& animation_,
		sf::FloatRect collision_, float height_, int health_);
	void move(Vectorf delta);
	void move_angled(int direction);
	void set_position(Vectorf new_position);
	void jump(bool run);
	void stop_jump();
	void update(float dt);
	void next_frame();
	Vectorf get_position();
	void rescale(float new_scale);
	void deal_damage(int amount);
	void set_max_health(int val);
	int get_max_health();
	void heal(int amount);
	void post_death();
};