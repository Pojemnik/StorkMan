#pragma once
#include "graphics.h"
#include "physics.h"
#include "platforms.h"

class Dynamic_entity : public Dynamic_animatable, public Physical
{
protected:
	bool reset_animation = false;
	float col_height;
	float jump_force_sum = 0;
	int edge_jump_buf = 0;
	int max_edge_jump = 10;
	int max_health;
	Vectorf last_pos = { 0,0 };
	Vectorf last_move_force = { 0,0 };

	void update_position(float dt);
	void flip(int s);
	void set_idle();
	void flip_if_needed();
	void edge_jump_update();
	void die();

public:
	int health;
	int last_dmgz_id = -1;

	Dynamic_entity(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
		std::vector<const Dynamic_animation*> a, sf::FloatRect rc,
		Animation_tree t, float h, float gs, float m, int hp);
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
};

class Player : public Dynamic_entity
{
public:
	Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
		std::vector<const Dynamic_animation*> a, sf::FloatRect rc,
		Animation_tree t, float h, float gs, float m, int hp);
	void attack(int type);
};