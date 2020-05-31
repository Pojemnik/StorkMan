#pragma once
#include "util.h"

class Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* tex;

public:
	std::vector<sf::Vertex> vertices;
	Vectorf pos;
	int layer;
	Texturable() = default;
	Texturable(Vectorf p, const sf::Texture* t,
		std::vector<sf::Vertex> points, int l);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void rescale(float ratio);
};

class Renderable : public sf::Drawable
{
protected:
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;
	int flipint;

public:
	int layer;
	Renderable() = default;
	Renderable(Vectorf p, const sf::Texture* t, float h, int l);
	Renderable(Vectorf p, const sf::Texture* t, float h, int l,int flip,float ang);
	void rescale(float ratio);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public sf::Drawable
{
protected:
	const std::vector<sf::Texture>* animation;
	std::vector<sf::Texture>::const_iterator it;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;
	int flipint;

public:
	int layer;
	Animatable() = default;
	Animatable(Vectorf p, const std::vector<sf::Texture>* a, float h, int l);
	Animatable(Vectorf p, const std::vector<sf::Texture>* a, float h, int l,
		int flip, float ang);
	void next_frame();
	void rescale(float ratio);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
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

struct Dynamic_animation
{
	const std::vector<std::vector<float>> key_frames;
	const std::vector<int> lengths;
	const bool repeat = false;

	Dynamic_animation(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r);
};

class Dynamic_animatable : public sf::Drawable
{
protected:
	const Animation_tree tree;
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
	void pre_draw();
	Dynamic_animatable(sf::Texture* texture, std::vector<sf::IntRect>& v,
		Vectorf p, std::vector<const Dynamic_animation*> a, Animation_tree t, float h, float gs);
	void next_frame();
	void rescale(float ratio);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};