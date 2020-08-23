#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
#include <cfloat>
#include "console.h"
#include "include/ctpl_stl.h"

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;
typedef std::string string;

const float PI = 3.1415927f;
const float eps = 0.0001f;
const int BOTTOM_LAYERS = 7;
const int MIDDLE_LAYERS = 3;
const int TOP_LAYERS = 2;

enum Entity_status {
	IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN,
	PUNCH_1, PUNCH_2, HIT, IN_AIR
};
enum Animation_status {
	A_IDLE = 0, A_MOVE, A_JUMP_IDLE,
	A_JUMP_RUN, A_JUMP_RUN2, A_PUNCH_1, A_PUNCH_2, A_HIT
};
enum Collidable_type { STATIC, MOVING };

struct Context
{
	bool draw_collisions = false;
	bool draw_map_vertices = false;
	bool draw_fps_counter = false;
	bool draw_hp = false;
	bool draw_damage_zones = false;
	bool night = true;
	bool god_mode = true;
	bool jump_available = true;
	float fps = 60.f;
	float gravity = 22;
	float jump_force = 775.f;
	float parallax = -1.5f;
	float parallax2 = -2.f;
	float global_scale = 32;	//[px/m]
	Vectorf max_move_speed = { 5,5 };
	float min_move_speed = 0.5;
	Vectorf move_speed_reduction = { 0.5f, 0.5f };
	Vectorf player_move_speed = { 8,0 };
	Vectorf background_position = { -1000,-2500 };
	float background_scale = 1.f;
	Vectorf layer2_position = { -1000, -1800 };
	float layer2_scale = 1.f;
	const Vectorf max_force = { 1000.f, 3000.0f };
	const Vectorf max_speed = { 20.f, 100.0f };
	sf::RenderStates bg_states, layer2_states, blurh_states, blurv_states,
		final_states, white_states;
	sf::Shader global;
	Vectori resolution = { 1024, 576 };
	sf::Text fps_counter;
	uint8_t darkness = 70;
	Console* console = nullptr;
	ctpl::thread_pool* thread_pool = nullptr;
	sf::Sound jump_idle;
	sf::Sound jump_run;
	sf::Sound aaa;
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
	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a,
		size_t i);
	inline float vector_dot_product(sf::Vector2f a, sf::Vector2f b);
	inline Vectorf saturate(Vectorf val, const Vectorf max_val);
	inline float deg_to_rad(float s);
	inline float rad_to_deg(float rdn);
	inline Vectorf rotate_vector(Vectorf vec, float ang);
	inline float ang_reduce(float ang);
	inline bool vectorf_compare(const Vectorf& a, const Vectorf& b);
	inline bool vectorf_binary_predicate(const Vectorf& a, const Vectorf& b);
	inline float convert_vector(const Vectorf& vec);
	inline Vectorf rotate_vector(Vectorf vec, Vectorf normalized_rotation_vector);
	inline std::string pass_or_default(std::string val, std::string default_val);
	Vectorf intersection_point(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b);
	bool intersection(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b);
	void save_texture(std::string path, sf::Texture* texture);
	sf::FloatRect mesh_to_rect(std::vector<sf::Vertex> vertices);

	inline std::string pass_or_default(std::string val, std::string default_val)
	{
		return val == "" ? default_val : val;
	}

	inline sf::Vector2f normalize(sf::Vector2f x, float l = 1)
	{
		return x / float(sqrt(x.x * x.x + x.y * x.y) * l);
	}

	inline sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a,
		size_t i)
	{
		Vectorf p1 = (*a)[i];
		Vectorf p2 = (i >= a->size() - 1) ? (*a)[0] : (*a)[i + 1];
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

	inline Vectorf saturate(Vectorf val, const Vectorf max_val)
	{
		if (val.x > max_val.x)
			val.x = max_val.x;
		if (val.x < -max_val.x)
			val.x = -max_val.x;
		if (val.y > max_val.y)
			val.y = max_val.y;
		if (val.y < -max_val.y)
			val.y = -max_val.y;
		return val;
	}

	inline float deg_to_rad(float s)
	{
		return(s / 180 * PI);
	}

	inline float rad_to_deg(float rdn)
	{
		return rdn / PI * 180;
	}

	inline float ang_reduce(float ang)
	{
		ang = fmod(ang, 360.0f);
		if (ang < 0)
			ang += 360.0f;
		return ang;
	}

	inline Vectorf rotate_vector(Vectorf vec, float ang)
	{
		float COS = cos(ang);
		float SIN = sin(ang);
		return { vec.x * COS -
				vec.y * SIN,
				vec.x * SIN +
				vec.y * COS };
	}
	inline Vectorf rotate_vector(Vectorf vec, Vectorf normalized_rotation_vector)
	{
		return { vec.x * normalized_rotation_vector.x -
				vec.y * normalized_rotation_vector.y,
				vec.x * normalized_rotation_vector.y +
				vec.y * normalized_rotation_vector.x };
	}

	inline bool vectorf_compare(const Vectorf& a, const Vectorf& b)
	{
		if (a.x != b.x)
		{
			return a.x > b.x;
		}
		else
		{
			return a.y > b.y;
		}
	}

	inline bool vectorf_binary_predicate(const Vectorf& a,
		const Vectorf& b)
	{
		return fabs(a.x - b.x) < 1 && fabs(a.y - b.y) < 1;
	}

	inline float convert_vector(const Vectorf& vec)
	{
		float SIN = vec.y / hypot(vec.x, vec.y);
		if (vec.x > 0)
			return SIN - 1;
		else
			return 1 - SIN;
	}

	template <class T>
	inline typename std::vector<T>::iterator increment_iterator(typename std::vector<T>::iterator a, std::vector<T>& vec)
	{
		a++;
		return a == vec.end() ? vec.begin() : a;
	}

	inline bool round_and_compare(Vectorf a, Vectorf b)
	{
		a = { float(round(a.x)), float(round(a.y)) };
		b = { float(round(b.x)), float(round(b.y)) };
		return a == b;
	}
}
