#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

const float PI = 3.1415927f;

const float global_scale = 35.84f; //[px/m]

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, PUNCH_1, HIT, IN_AIR };
enum Animation_status { A_IDLE = 0, A_MOVE, A_JUMP_IDLE, A_JUMP_RUN, A_JUMP_RUN2, A_PUNCH_1, A_HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };

const Vectorf max_force = { 1000.f, 1000.0f };

struct Context
{
	bool draw_collisions = true;
	float fps = 60.f;
	float gravity = .5f;
	float jump_force = 20.f;
	float parrallax = -1.5f;
	Vectorf max_move_speed = { 5,5 };
	Vectorf min_move_speed = { 1,0 };
	Vectorf move_speed_reduction = { 0.5f, 0.5f };
	Vectorf player_move_speed = { 0.1,0 };
};

extern struct Context context;

namespace util
{
	struct xyr
	{
		Vectorf pos;
		float r;
	};

	struct command
	{
		std::string name;
		std::vector<std::string> args;
	};

	inline sf::Vector2f normalize(sf::Vector2f x, float l);
	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a, int i);
	inline float vector_dot_product(sf::Vector2f a, sf::Vector2f b);
	Vectorf saturate(Vectorf val, const Vectorf max_val);
	float rdn(float s);
	float ang_reduce(float ang);
	void print_argument_number_error(int correct_number);
	void print_incorrect_argument_error(std::string command, std::string what);
	command get_command();
	void execute_command(util::command cmd);

	inline sf::Vector2f normalize(sf::Vector2f x, float l)
	{
		return x / float(sqrt(x.x * x.x + x.y * x.y) * l);
	}

	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a, int i)
	{
		sf::Vector2f p1 = (*a)[i], p2 = (i >= a->size() - 1) ? (*a)[0] : (*a)[i + 1];
		return util::normalize({ p1.y - p2.y,p2.x - p1.x }, 1);
	}

	inline float vector_dot_product(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}


	template <typename T> inline int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
}
