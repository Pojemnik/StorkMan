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
#include "../include/ctpl_stl.h"

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
	PUNCH_1, PUNCH_2, IN_AIR, DIE, HIT
};

enum Animation_status {
	A_IDLE = 0, A_MOVE, A_JUMP_IDLE,
	A_JUMP_RUN, A_JUMP_RUN2, A_PUNCH_1, A_PUNCH_2, A_DIE, A_HIT
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
	bool god_mode = false;
	bool jump_available = true;
	bool generate_light = false;
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
	const Vectori default_resolution = { 1024, 576 };
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

	//Vectors
	sf::Vector2f normalize(sf::Vector2f x, float l = 1);
	sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>* a,
		size_t i);
	float vector_dot_product(sf::Vector2f a, sf::Vector2f b);
	float vector_cross_product(sf::Vector2f a, sf::Vector2f b);
	bool round_and_compare(Vectorf a, Vectorf b);
	Vectorf saturate(Vectorf val, const Vectorf max_val);
	bool vectorf_compare(const Vectorf& a, const Vectorf& b);
	bool vectorf_binary_predicate(const Vectorf& a, const Vectorf& b);
	float convert_vector(const Vectorf& vec);
	Vectorf rotate_vector(Vectorf vec, Vectorf normalized_rotation_vector);
	Vectorf rotate_vector(Vectorf vec, float ang);

	//Numbers
	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T> T sq(T a)
	{
		return a * a;
	}

	//Angles
	float deg_to_rad(float s);
	float rad_to_deg(float rdn);
	float ang_reduce(float ang);

	//Geometry
	Vectorf intersection_point(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b);
	bool intersection(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b);
	bool contained_in_polygon(Vectorf point, float max_x, const std::vector<Vectorf>& polygon);

	//Misc
	template <class T> typename std::vector<T>::iterator increment_iterator(
		typename std::vector<T>::iterator a, const std::vector<T>& vec)
	{
		a++;
		return a == vec.end() ? vec.begin() : a;
	}

	template <class T> typename std::vector<T>::const_iterator increment_iterator(
		typename std::vector<T>::const_iterator a, const std::vector<T>& vec)
	{
		a++;
		return a == vec.cend() ? vec.cbegin() : a;
	}

	std::string pass_or_default(std::string val, std::string default_val);
	void save_texture(std::string path, sf::Texture* texture);
	sf::FloatRect mesh_to_rect(std::vector<sf::Vertex> vertices);
}
