#pragma once
#include <queue>
#include "util.h"
#include "animation_part.h"
#include "animation_core.h"

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
	virtual void change_textures_set(int set) = 0;
	virtual ~Animation() {}
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
	std::unordered_map<std::pair<int, int>, std::pair<Animation_index,
		std::vector<int>>, pair_hash> alternative_animations;
	Frame_info frame_info;

	Animation_tree() = default;
	Animation_tree(int count_, int i_count, Frame_info info);
};

class Key_frame_animation : public Animation
{
protected:
	const Animation_tree tree;
	std::vector<std::shared_ptr<Animation_part>> parts;
	std::vector<sf::Sprite> parts_sprites;
	sf::RenderTexture tex;
	std::vector<const Dynamic_animation_struct*> animations;
	int key;
	float time_to_next_frame;
	const std::vector<float>* last_key;
	const std::vector<float>* next_key;
	std::vector<float> actual_frame;
	Animation_index animation;
	const float ANIMATION_CHANGE_DELTA = 5.0f;

	Vectorf count_pos(Vectorf start,
		Vectori translation1, float a1,
		Vectori translation2, float a2);
	void animate(std::vector<float> arr);
	void increment_key();
	void pre_draw();

public:
	Key_frame_animation(std::vector<std::shared_ptr<Animation_part>>&& parts_,
		std::vector<const Dynamic_animation_struct*> animations_, const Animation_tree& tree_);
	Key_frame_animation(std::vector<std::shared_ptr<Animation_part>> parts_,
		std::vector<const Dynamic_animation_struct*> animations_, const Animation_tree& tree_);
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_index get_current_animation() const;
	Frame_info get_frame_info() const;
	void change_textures_set(int set);
};

class Static_animation : public Animation
{
protected:
	Static_animation_part part;

public:
	Static_animation(Static_animation_struct& animation_, float time_offset);
	~Static_animation() {}
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_index get_current_animation() const;
	Frame_info get_frame_info() const;
	void change_textures_set(int set);
};

class One_frame_animation : public Animation
{
protected:
	const sf::Texture* tex;
public:
	One_frame_animation(const sf::Texture*);
	void next_frame(float dt);
	const sf::Texture* const get_texture();
	void set_animation(Animation_index a);
	Animation_index get_current_animation() const;
	Frame_info get_frame_info() const;
	void change_textures_set(int set);
};
