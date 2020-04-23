#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <list>
#include <array>
#include <queue>
#include <fstream>
#include <map>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

const float PI = 3.1415927f;

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN,
	PUNCH_1, PUNCH_2, HIT, IN_AIR };
enum Animation_status { A_IDLE = 0, A_MOVE, A_JUMP_IDLE,
	A_JUMP_RUN, A_JUMP_RUN2, A_PUNCH_1, A_PUNCH_2, A_HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };

struct Context
{
	bool draw_collisions = false;
	bool draw_map_vertices = false;
	bool draw_fps_counter = false;
	float fps = 60.f;
	float gravity = 26;
	float jump_force = 870.f;
	float parrallax = -1.5f;
	float parrallax2 = -2.f;
	float global_scale = 35.84f;	//[px/m]
	Vectorf max_move_speed = { 5,5 };
	float min_move_speed = 1;
	Vectorf move_speed_reduction = { 0.5f, 0.5f };
	Vectorf player_move_speed = { 1,0 };
	Vectorf background_position = { -1000,-2500 };
	float background_scale = 1.f;
	Vectorf layer2_position = { -1000, -1800 };
	float layer2_scale = 1.f;
	const Vectorf max_force = { 1000.f, 3000.0f };
	sf::Shader blurh, blurv, shade;
	sf::RenderTexture lightmap, lm2, lm3;
	sf::RenderStates bg_states, layer2_states,
		map_states, blurh_states, blurv_states, final_states, light_states;
	Vectori resolution = { 1024, 576 };
	sf::Font arial;
	sf::Text fps_counter;
};

extern struct Context context;

namespace util
{
	struct xyr
	{
		Vectorf pos;
		float r;
	};

	inline sf::Vector2f normalize(sf::Vector2f x, float l);
	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a, size_t i);
	inline float vector_dot_product(sf::Vector2f a, sf::Vector2f b);
	Vectorf saturate(Vectorf val, const Vectorf max_val);
	float deg_to_rad(float s);
	float rad_to_deg(float rdn);
	Vectorf rotate_vector(Vectorf vec, float ang);
	float ang_reduce(float ang);
	bool vectorf_compare(const Vectorf& a, const Vectorf& b);
	bool vectorf_binary_predicate(const Vectorf& a, const Vectorf& b);

	inline sf::Vector2f normalize(sf::Vector2f x, float l)
	{
		return x / float(sqrt(x.x * x.x + x.y * x.y) * l);
	}

	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a, size_t i)
	{
		sf::Vector2f p1 = (*a)[i], p2 = (i >= a->size() - 1) ? (*a)[0] : (*a)[i + 1];
		return util::normalize({ p1.y - p2.y,p2.x - p1.x }, 1);
	}

	inline float vector_dot_product(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float vector_cross_product(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.y - a.y * b.x;
	}

	template <typename T> inline int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T> inline T sq(T a)
	{
		return a * a;
	}
}
