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

//Linear_moving_platform, Pendulum, inne?
class Moving_platform : public Texturable, public Physical
{
public:
	bool visible = true;
	Moving_platform(const sf::Texture* tex, Vectorf pos, std::vector<sf::Vertex> pts,
		int layer_, bool visible_ = true);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	virtual void update_position(float dt) = 0;
};

class Pendulum : public Moving_platform
{
public:
	Pendulum(const sf::Texture* pen_tex, const sf::Texture* line_tex_,
		std::vector<Vectorf> attach, std::vector<sf::Vertex> points_,
		float line_l, Vectorf pos_, float angle_, int layer_, bool visible_ = true);
	void update(float dt);
	void update_position(float dt);
	void move(Vectorf delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Vectorf anchor;
	Vectorf angle = { 1,0 };
	float a_speed = 0;
	float rad_angle = PI / 3;
	float line_len;
	float scale;
	std::vector<std::pair<sf::Sprite, Vectorf>> lines;
	const sf::Texture* line_tex;
};

struct Linear_move
{
	std::vector<std::pair<Vectorf, float>> points;
	std::vector<std::pair<Vectorf, float>>::iterator it;
};

class Linear_moving_platform : public Moving_platform
{
private:
	Linear_move move_data;
	float time = 0;
public:
	Linear_moving_platform(Linear_move path, const sf::Texture* tex_, Vectorf pos_,
		std::vector<sf::Vertex> pts, int layer_, bool visible_ = true);
	Linear_moving_platform(const Linear_moving_platform& lmp);
	void move(Vectorf delta);
	void update(float dt);
	void update_position(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};