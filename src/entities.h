#pragma once
#include "graphics.h"
#include "physics.h"
#include "platforms.h"
#include "logic.h"
#include "animations.h"

class Entity : public Animatable, public Updatable, public Collidable, public Transformable
{
protected:
	sf::Sprite sprite;
	float height;
	float mass;
	std::unique_ptr<Animation> animation;
	Vectorf pos;
	int direction = 1;//x sign
	bool reset_animation = false;
	float col_height;
	float jump_force_sum = 0;
	int edge_jump_buf = 0;
	int max_edge_jump = 10;
	int max_health;
	Vectorf last_pos = { 0,0 };
	Vectorf last_move_force = { 0,0 };
	Entity_status status;
	Entity_status last_status;

	void update_position(float dt);
	void set_idle();
	void flip_if_needed();
	void edge_jump_update();
	void die();

public:
	int health;
	int last_dmgz_id = -1;

	Entity(Vectorf p, std::unique_ptr<Animation>&& animation_,
		sf::FloatRect collision_, float height_, float mass_, int health_);
	void move(Vectorf delta);
	void move_angled(int direction);
	void set_position(Vectorf new_position);
	void jump(bool run);
	void stop_gjump();
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

class Player : public Entity
{
public:
	Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
		std::vector<const Dynamic_animation_struct*> a, sf::FloatRect rc,
		Animation_tree t, float h, float gs, float m, int hp);
	void attack(int type);
	void post_death();
};