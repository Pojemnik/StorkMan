#pragma once
#include <queue>
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

class Animatable
{
public:
	virtual void update_frame() = 0;
	virtual void next_frame(float dt) = 0;
};

class Animation
{
public:
	virtual void next_frame(float dt) = 0;
	virtual const sf::Texture* const get_texture() = 0;
	virtual void set_animation(Animation_index a) = 0;
	virtual Animation_index get_current_animation() const = 0;
	virtual Frame_info get_frame_info() const = 0;
	virtual ~Animation() {}
};

struct Animation_node
{
	std::array<Vectori, 2> delta_pos;
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

struct Animation_tree
{
	int count;
	int independent_count;
	std::vector<int> position_of_element_in_animation_array;	//Longest name ever
	std::vector<Animation_node> nodes;
	std::vector<std::vector<int>> tree;
	int root;
	std::unordered_map<std::pair<int, int>, std::pair<Animation_index,
		std::vector<int>>, pair_hash> alternative_animations;
	Frame_info frame_info;

	Animation_tree() = default;
	Animation_tree(int _count, int i_count, Frame_info info);
};

struct Dynamic_animation_struct
{
	const std::vector<std::vector<float>> key_frames;
	const std::vector<int> lengths;
	const bool repeat = false;

	Dynamic_animation_struct(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r);
};

struct Static_animation_struct
{
	const std::vector<const sf::Texture*>* animation;
	std::vector<const sf::Texture*>::const_iterator it;
	float frame_time;

	Static_animation_struct(const std::vector<const sf::Texture*>* animation_, float frame_time_);
	Static_animation_struct(Static_animation_struct& a);
};

class Dynamic_animation : public Animation
{
protected:
	const Animation_tree tree;
	std::vector<sf::Sprite> parts;
	sf::RenderTexture tex;
	std::vector<const Dynamic_animation_struct*> animations;
	int key;
	float time_to_next_frame;
	const std::vector<float>* last_key;
	const std::vector<float>* next_key;
	std::vector<float> actual_frame;
	Animation_index animation;
	const int ANIMATION_CHANGE_DELTA = 5;

	Vectorf count_pos(Vectorf start,
		Vectori translation1, float a1,
		Vectori translation2, float a2);
	void animate(std::vector<float> arr);
	void increment_key();
	void pre_draw();

public:
	Dynamic_animation(const sf::Texture* texture_, std::vector<sf::IntRect>& part_sizes,
		std::vector<const Dynamic_animation_struct*> animations_, const Animation_tree& tree_);
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_index get_current_animation() const;
	Frame_info get_frame_info() const;
};

class Static_animation : public Animation
{
protected:
	Static_animation_struct animation;
	sf::Sprite sprite;
	float time = 0;
	Frame_info frame_info;

public:
	Static_animation(Static_animation_struct& animation_, float time_offset);
	~Static_animation() {}
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_index get_current_animation() const;
	Frame_info get_frame_info() const;
};