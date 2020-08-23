#pragma once
#include "graphics.h"
#include "physics.h"
#include "platforms.h"

struct Wall : public Texturable
{
	Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
		int layer);
};

struct Object : public Renderable
{
	Object(Vectorf p, const sf::Texture* t, float h, int layer);
	Object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
		float ang);
};

class Zone
{
public:
	std::vector<Vectorf> vertices;
	Vectorf pos;
	float max_x;

	Zone(std::vector<Vectorf>& vert, Vectorf p);
	Zone(const std::vector<Vectorf>& vert, Vectorf p);
	bool is_inside(Vectorf p);
};

class Damage_zone : Zone
{
private:
	std::vector<std::pair<int, int>> damage;
	float time = 0;

public:
	std::vector<std::pair<int, int>>::iterator current_damage;

	void update(float dt);
	Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
		std::vector<std::pair<int, int>>& dmg);
	Damage_zone(const Damage_zone &dmgz);
};

class Moving_object : public Object
{
	Linear_move move_data;
	float time = 0;
	Vectorf move_pos;

public:
	Moving_object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
		float ang,Linear_move path);
	Moving_object(const Moving_object&);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animated_object : public Animatable
{
public:
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer, int fr);
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer, int fr, int flip, float ang, int frame_delta);
	void next_frame();

private:
	int frames_diff;
	int frame_counter = 0;
};

class Moving_animated_object : public Animated_object
{
	Linear_move move_data;
	float time = 0;
	Vectorf pos;
public:
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Dynamic_entity : public Dynamic_animatable, public Physical
{
protected:
	bool reset_animation = false;
	float col_height;
	float jump_force_sum = 0;
	int edge_jump_buf = 0;
	int max_edge_jump = 10;
	Vectorf last_pos = { 0,0 };

	void update_position(float dt);
	void flip(int s);
	void set_idle();

public:
	const int MAX_HEALTH;
	int health;
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
	void die();
};

class Player : public Dynamic_entity
{
public:
	Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
		std::vector<const Dynamic_animation*> a, sf::FloatRect rc,
		Animation_tree t, float h, float gs, float m, int hp);
	void attack(int type);
};