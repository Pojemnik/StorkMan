#pragma once
#include <queue>
#include "util.h"

enum class Animation_index : int
{
	DEFAULT = 0, IDLE, MOVE, JUMP_IDLE, JUMP_RUN_1, JUMP_RUN_2, PUNCH_1, PUNCH_2,
	DIE, HIT
};

struct Animation_info
{
	Animation_index index;
	float time_left;
	int current_key;

	Animation_info() = default;
	Animation_info(Animation_index index_, float time_left_, int current_key_);
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
	virtual Animation_info get_animation_info() const = 0;
};

struct Animation_node
{
	std::array<Vectori, 2> delta_pos;
};

struct Animation_tree
{
	int count;
	int independent_count;
	std::vector<int> position_of_element_in_animation_array;	//Longest name ever
	std::vector<Animation_node> nodes;
	std::vector<std::vector<int>> tree;
	int root;

	Animation_tree() = default;
	Animation_tree(int _count, int i_count);
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
	const std::vector<sf::Texture>* animation;
	std::vector<sf::Texture>::const_iterator it;
	float frame_time;

	Static_animation_struct(const std::vector<sf::Texture>* animation_, float frame_time_);
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
	float time_to_animation_end;
	const std::vector<float>* last_key;
	const std::vector<float>* next_key;
	std::vector<float> actual_frame;
	Animation_index animation;
	Animation_index last_animation;
	const int ANIMATION_CHANGE_DELTA = 5;

	Vectorf count_pos(Vectorf start, float size1, float size2,
		Vectori translation1, float a1,
		Vectori translation2, float a2);
	void animate(std::vector<float> arr);
	virtual void post_death() = 0;
	void increment_key();
	void pre_draw();

public:
	Dynamic_animation(sf::Texture* texture_, std::vector<sf::IntRect>& part_sizes,
		std::vector<const Dynamic_animation_struct*> animations_, Animation_tree tree_);
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_info get_animation_info() const;
};

class Static_animation : public Animation
{
protected:
	Static_animation_struct animation;
	sf::Sprite sprite;
	float time = 0;

public:
	Static_animation(Static_animation_struct& animation_, float time_offset);
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_info get_animation_info() const;
};