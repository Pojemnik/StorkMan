#pragma once
#include <SFML/Graphics.hpp>
#include "animation_core.h"

class Animation_part
{
public:
	virtual const sf::Texture* const get_texture() = 0;
	virtual void set_image(int image) { (void)image; };
	virtual void advance(float dt) { (void)dt; };
	virtual Vectori get_texture_size() = 0;
	virtual ~Animation_part() {}
};

class Const_animation_part : public Animation_part
{
	const sf::Texture* tex;
public:
	Const_animation_part(sf::Texture* tex_) : tex(tex_){}
	const sf::Texture* const get_texture();
	virtual Vectori get_texture_size();
};

class Multi_texture_animtion_part : public Animation_part
{
	const std::vector<const sf::Texture*> textures;
	const sf::Texture* current_tex;

public:
	Multi_texture_animtion_part(const std::vector<const sf::Texture*> textures_, int start = 0);
	const sf::Texture* const get_texture();
	void set_image(int image);
	virtual Vectori get_texture_size();
};

class Static_animation_part : public Animation_part
{
	Static_animation_struct animation;
	Frame_info frame_info;
	float time = 0;
	const float time_offset;

public:
	Static_animation_part(Static_animation_struct& animation_, float time_offset);
	virtual const sf::Texture* const get_texture();
	virtual void advance(float dt);
	virtual Vectori get_texture_size();
	virtual void reset();

	friend class Static_animation;
};