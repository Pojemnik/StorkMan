#pragma once
#include <cmath>
#include <array>

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
	std::vector<sf::Vertex> vertices;
	Vectorf pos;

public:
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

class Dynamic_animation
{
public:
	const std::vector<std::array<float, 21>> key_frames;
	const std::vector<int> lengths;

	Dynamic_animation(std::vector<std::array<float, 21>>& kf, std::vector<int>& l);
};

class Dynamic_animatable : public sf::Drawable
{
protected:
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
	const std::array<float, 21>* last_key;
	const std::array<float, 21>* next_key;
	std::array<float, 21> actual_frame;
	Animation_status animation_status;
	Animation_status last_animation_status;
	Entity_status status;
	Entity_status last_status;
	const int ANIMATION_CHANGE_DELTA = 5;
	Vectorf count_pos(Vectorf start, float size1, float size2,
		float translation_x1, float translation_y1, float angle1,
		float translation_x2, float translation_y2, float angle2);
	void animate(std::array<float, 21> arr);
	void animate(float x, float y, float r, float KLArGLO, float BRZrKLA,
		float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL,
		float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD,
		float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY,
		float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO);
public:
	Dynamic_animatable(sf::Texture* texture,std::vector<sf::IntRect>& v, Vectorf p, std::vector<const Dynamic_animation*> a, float h, float gs);
	void next_frame();
	void set_animation(Animation_status s);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
