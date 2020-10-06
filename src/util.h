#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

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

struct Context
{
	bool draw_map_vertices = false;
	bool draw_fps_counter = false;
	bool draw_hp = false;
	bool draw_damage_zones = false;
	bool god_mode = false;
	float fps = 60.f;
	float gravity = 0.3f;
	float global_scale = 32;	//[px/m]
	Vectori resolution = { 1024, 576 };
	const Vectori default_resolution = { 1024, 576 };
	std::unique_ptr<Console> console;
	std::unique_ptr<ctpl::thread_pool> thread_pool;
	Vectori level_size = { 100, 100 };
	bool window_focus = false;
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
	sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>& a,
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
		typename std::vector<T>::iterator a, std::vector<T>& vec)
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

	std::stringstream remove_comments(std::ifstream& file);
	std::string pass_or_default(std::string val, std::string default_val);
	void save_texture(std::string path, sf::Texture* texture);
	sf::FloatRect mesh_to_rect(const std::vector<Vectorf>& vertices);
	sf::FloatRect mesh_to_rect(const std::vector<sf::Vertex>& vertices);
}
