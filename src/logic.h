#pragma once
#include "util.h"

class Simple_AI
{
public:
	virtual void calc_pos(float dt) = 0;
	virtual sf::Transform get_pos() = 0;
	virtual ~Simple_AI() {}
};

class Linear_AI : public Simple_AI
{
	sf::Vector2f pos;
	float time;
	std::vector<std::pair<Vectorf, float>> points;
	std::vector<std::pair<Vectorf, float>>::const_iterator it;
public:
	void calc_pos(float dt);
	sf::Transform get_pos();
	Linear_AI(std::vector<std::pair<Vectorf, float>> points_, float time_offset);
};

class Swing_AI : public Simple_AI
{
	const float GRAVITY = -0.008f;
	sf::Vector2f pos;
	const float line_len;
	float a_speed = 0;
	float rad_angle = 0;
	float time=0;
public:
	void calc_pos(float dt);
	sf::Transform get_pos();
	Swing_AI(const float line_len_, float angle_);
};

class Swing_rotation_AI : public Simple_AI
{
	const float GRAVITY = -0.008f;
	sf::Vector2f pos;
	const float line_len;
	float a_speed = 0;
	float rad_angle = 0;
	Vectorf pivot;
	float time=0;

public:
	void calc_pos(float dt);
	sf::Transform get_pos();
	Swing_rotation_AI(const float line_len_, float angle_, Vectorf pivot_);
};

class Accelerated_linear_AI : public Simple_AI
{
	sf::Vector2f pos;
	float time;
	std::vector<std::tuple<Vectorf, float, float>> points;
	std::vector<std::tuple<Vectorf, float, float>>::const_iterator it;
	float v0;
	float s0;

public:
	void calc_pos(float dt);
	sf::Transform get_pos();
	Accelerated_linear_AI(std::vector<std::tuple<Vectorf, float, float>> points_, float time_offset);
};