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

struct Linear_move
{
	std::vector<Vectorf> points;
	float speed;
};

//Linear_moving_platform, Pendulum, inne?
class Moving_platform : public Texturable, public Physical
{
public:
	Moving_platform(const sf::Texture* tex, Vectorf pos, std::vector<sf::Vertex> pts,
		int layer);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	virtual void update_position(float dt) = 0;
};

class Pendulum : public Moving_platform
{
public:
	Pendulum(const sf::Texture* pen_tex, const sf::Texture* line_tex_,
		std::vector<Vectorf> attach, std::vector<sf::Vertex> points_,
		float line_l, Vectorf pos_, float angle_, int layer_);
	void update(float dt);
	void update_position(float dt);
	void move(Vectorf delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Vectorf anchor;
	Vectorf angle = {1,0};
	float a_speed = 0;
	float rad_angle = PI/3;
	float line_len;
	float scale;
	std::vector<std::pair<sf::Sprite, Vectorf>> lines;
	const sf::Texture* line_tex;
};