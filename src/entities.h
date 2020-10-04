#pragma once
#include <stack>
#include <variant>
#include <iostream>
#include <assert.h>
#include "physics.h"
#include "logic.h"
#include "animations.h"
#include "messaging.h"
#include "control.h"

enum class Entity_state_info {PUSH, POP, REPLACE, NONE};
enum class Entity_type { PLAYER };

class Entity_state_machine;

class Entity_id
{	
	Entity_type type;
	int id;

public:
	Entity_id(Entity_type type_);
	Entity_type get_type() const;
	int get_id() const;
};

class Entity : public Updatable, public Collidable, public Message_sender, public Renderable
{
protected:
	float move_speed = 4.f;
	float jump_force = 1.5f;
	int last_direction = 1;
	Physical physical;
	sf::Sprite sprite;
	std::pair<float, int> height;
	std::unique_ptr<Animation> animation;
	std::unique_ptr<Entity_state_machine> state;
	int max_health;
	float scale;
	sf::RectangleShape coll_shape;
	bool draw_collision = false;

	void flip(int direction);

public:
	std::unique_ptr<Controller> controller;
	int health;
	int direction = 1;//x sign
	Vectorf collision_vector;
	Surface_type surface;
	bool on_ground;
	Entity_id id;

	void set_draw_collision(bool draw);
	void set_jump_force(float new_force);
	void set_move_speed(float new_speed);
	const Collision* const get_collision() const;
	void resolve_collision(const std::vector<std::shared_ptr<const Collidable>>& others);
	void resolve_collision(const Collidable& other);
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
		std::unique_ptr<Controller>&& controller_, std::pair<float, int> height_,
		int health_, Entity_type type);
};

struct Compare_entities
{
	bool operator() (Entity const* lhs, Entity const* rhs) const
	{
		return (lhs->id.get_id() == rhs->id.get_id());
	}
};

struct Entity_state
{
	virtual void enter(Entity& entity) = 0;
	virtual void exit(Entity& entity) { (void)entity; /*Unused*/ };
	virtual std::pair<Entity_state*, Entity_state_info> update(Entity& entity, float dt) = 0;
	virtual ~Entity_state() {};
};

class Entity_state_machine
{
	std::stack<Entity_state*> state_stack;

public:
	virtual void update(Entity& entity, float dt);
	Entity_state_machine(Entity_state* state);
};
