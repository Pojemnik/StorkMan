#include "animation_part.h"

Static_animation_part::Static_animation_part(Static_animation_struct& animation_, float time_offset) :
animation(animation_), time(time_offset), frame_info(Vectori((*animation_.it)->getSize()),
	Vectori((*animation_.it)->getSize()), { 0,0 })
{
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
	advance(.0f);
}

const sf::Texture* const Static_animation_part::get_texture()
{
	return *animation.it;
}

void Static_animation_part::advance(float dt)
{
	time += dt;
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
}

Vectori Static_animation_part::get_texture_size()
{
	return frame_info.frame_size;
}

const sf::Texture* const Const_animation_part::get_texture()
{
	return tex;
}

Vectori Const_animation_part::get_texture_size()
{
	return static_cast<Vectori>(tex->getSize());
}

Triggered_animtion_part::Triggered_animtion_part(
	const std::vector<const sf::Texture*> textures_, int start) : 
	textures(textures_)
{
	current_tex = textures.at(start);
}

const sf::Texture* const Triggered_animtion_part::get_texture()
{
	return current_tex;
}

void Triggered_animtion_part::set_image(int image)
{
	current_tex = textures.at(image);
}

Vectori Triggered_animtion_part::get_texture_size()
{
	return static_cast<Vectori>(current_tex->getSize());
}
