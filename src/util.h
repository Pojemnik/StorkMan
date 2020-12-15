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
const int TOTAL_LAYERS = BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS;

class Id_generator
{
	std::mutex mutex;
	int next_id = 0;

public:
	int get_id();
};

struct Context
{
	bool draw_map_vertices = false;
	bool draw_fps_counter = false;
	bool draw_hp = false;
	bool draw_damage_zones = false;
	bool god_mode = false;
	float fps = 60.f;
	float gravity = 0.3f;
	float global_scale = 64;	//[px/m]
	Vectori resolution = { 1920, 1080 };
	Vectorf player_pos;
	std::unique_ptr<Console> console;
	std::unique_ptr<ctpl::thread_pool> thread_pool;
	Vectori level_size = { 50, 50 };
	bool window_focus = false;
	Id_generator id_generator;
	bool editor_mode = false;
};

extern struct Context context;

namespace util
{
	class Color_generator
	{
	private:
		std::vector<sf::Color> colors;
		std::vector<sf::Color>::const_iterator it;

	public:
		Color_generator(string path);
		sf::Color get_color();
	};

	struct xyr
	{
		Vectorf pos;
		float r;
	};

	template<typename T>
	inline void hash_combine(std::size_t& seed, const T& val)
	{
		std::hash<T> hasher;
		seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct pair_hash
	{
		template<typename S, typename T>
		inline size_t operator()(const std::pair<S, T>& val) const
		{
			size_t seed = 0;
			hash_combine(seed, val.first);
			hash_combine(seed, val.second);
			return seed;
		}
	};

	//Vectors
	sf::Vector2f normalize(sf::Vector2f x, float l = 1);
	sf::Vector2f get_axis_normal(const std::vector<sf::Vector2f>& a,
		size_t i);
	float vector_dot_product(sf::Vector2f a, sf::Vector2f b);
	float vector_cross_product(sf::Vector2f a, sf::Vector2f b);
	bool round_and_compare(Vectorf a, Vectorf b);
	bool round_and_compare(Vectorf a, Vectorf b, float eps);
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
	int orientation(Vectorf p, Vectorf q, Vectorf r);
	bool on_segment(Vectorf p, Vectorf q, Vectorf r);
	bool are_colinear(Vectorf p, Vectorf q, Vectorf r, float epsilon = eps);

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
	sf::FloatRect merge_bounds(const sf::FloatRect& first, const sf::FloatRect& second);
}
