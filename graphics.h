#pragma once
#include <cmath>
#include <array>
#include <queue>

#include "util.h"

class Animation
{
public:
	Vectorf center;
	const std::vector<sf::Texture> content;
	const sf::FloatRect rect_collision;
	std::vector<sf::Texture>::const_iterator begin() const;
	std::vector<sf::Texture>::const_iterator end() const;
	Animation() = default;
	Animation(std::vector<sf::Texture>& a, Vectorf c, sf::FloatRect rect_col);
};

class Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* tex;

public:
	std::vector<sf::Vertex> vertices;
	Vectorf pos;
	Texturable();
	Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Renderable : public sf::Drawable
{
protected:
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;

public:
	Renderable() = default;
	Renderable(Vectorf p, sf::Texture* t, float h);
	void rescale(float gs);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public sf::Drawable
{
protected:
	const Animation* tex;
	Vectorf pos;
	std::vector<sf::Texture>::const_iterator it;
	sf::Sprite sprite;
	int direction = 1;//x sign
	float height; //[m]
	float scale;

public:
	Animatable();
	Animatable(Vectorf p, const Animation* t, float h, float gs);
	void next_frame();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

struct Animation_node
{	
	std::array<Vectori, 2> delta_pos;
};

class Animation_tree
{
public:
	int count;
	int independent_count;
	std::vector<int> position_of_element_in_animation_array;	//Longest name ever
	std::vector<Animation_node> nodes;
	std::vector<std::vector<int>> tree;
	int root;
	Animation_tree() = default;
	Animation_tree(int _count, int i_count);
};

class Dynamic_animation
{
public:
	const std::vector<std::vector<float>> key_frames;
	const std::vector<int> lengths;
	const bool repeat = false;

	Dynamic_animation(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r);
};

class Dynamic_animatable : public sf::Drawable
{
protected:
	const Animation_tree tree;
	Vectori last_collision_direction = { 0,0 };
	Vectorf pos;
	std::vector<sf::Sprite> parts;
	int direction = 1;//x sign
	float height; //[m]
	float scale;
	sf::RenderTexture tex;
	sf::Sprite sprite;
	std::vector<const Dynamic_animation*> animations;
	int key;
	int frames_delta;
	const std::vector<float>* last_key;
	const std::vector<float>* next_key;
	std::vector<float> actual_frame;
	Animation_status animation_status;
	Animation_status last_animation_status;
	Entity_status status;
	Entity_status last_status;
	const int ANIMATION_CHANGE_DELTA = 5;
	float maxy;
	float miny;

	Vectorf count_pos(Vectorf start, float size1, float size2,
		Vectori translation1, float a1,
		Vectori translation2, float a2);
	void animate(std::vector<float> arr);
	void set_animation(Animation_status s);
public:
	Dynamic_animatable(sf::Texture* texture, std::vector<sf::IntRect>& v,
		Vectorf p, std::vector<const Dynamic_animation*> a, Animation_tree t, float h, float gs);
	void next_frame();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};