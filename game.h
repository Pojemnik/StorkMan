#pragma once
#include "graphics.h"
#include "physics.h"

struct Platform : public Texturable, public Collidable
{
	bool visible = true;
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
		int layer, bool v);
	void rescale(float ratio);
};

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

struct Animated_object : public Animatable
{
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer);
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer, int flip, float ang);
};

class Dynamic_entity : public Dynamic_animatable, public Physical
{
protected:
	bool reset_animation = false;
	float col_height;

	void update_position(float dt);
	void flip(int s);
	void set_idle();

public:
	Dynamic_entity(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v, std::vector<const Dynamic_animation*> a, sf::FloatRect rc, Animation_tree t, float h, float gs, float m);
	void move(Vectorf delta);
	void move_angled(int direction);
	void set_position(Vectorf new_position);
	void jump(bool run);
	void update(float dt);
	void next_frame();
	Vectorf get_position();
	void rescale(float new_scale);
};

class Player : public Dynamic_entity
{
public:
	Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
		std::vector<const Dynamic_animation*> a, sf::FloatRect rc,
		Animation_tree t, float h, float gs, float m);
	void attack(int type);
};