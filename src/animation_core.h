#pragma once
#include <SFML/Graphics.hpp>
#include "util.h"

enum class Animation_index : int
{
	DEFAULT = 0, IDLE, MOVE, JUMP_IDLE, JUMP_RUN, PUNCH_1, PUNCH_2,
	DIE, HIT, ADDITONAL_1
};

const int ANIMATIONS_N = 10;

struct Frame_info
{
	Vectori part_size;
	Vectori frame_size;
	Vectori offset;
	Vectorf character_position;

	Frame_info() = default;
	Frame_info(Vectori part_size_, Vectori frame_size_, Vectori offset_);
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

struct Dynamic_animation_struct
{
	const std::vector<std::vector<float>> key_frames;
	const std::vector<float> lengths;
	const bool repeat = false;

	Dynamic_animation_struct(std::vector<std::vector<float>>& kf, std::vector<float>& l, bool r);
};

struct Static_animation_struct
{
	const std::vector<const sf::Texture*>* animation;
	std::vector<const sf::Texture*>::const_iterator it;
	float frame_time;

	Static_animation_struct(const std::vector<const sf::Texture*>* animation_, float frame_time_);
	Static_animation_struct(Static_animation_struct& a);
};
