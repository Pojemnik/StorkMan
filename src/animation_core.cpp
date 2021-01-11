#include "animation_core.h"

Dynamic_animation_struct::Dynamic_animation_struct(std::vector<std::vector<float>>& kf,
	std::vector<float>& l, bool r)
	: key_frames(kf), lengths(l), repeat(r) {}

Static_animation_struct::Static_animation_struct(
	const std::vector<const std::array<const sf::Texture*, 3>*>* animation_,
	float frame_time_) :
	animation(animation_), frame_time(frame_time_)
{
	it = animation->cbegin();
}

Static_animation_struct::Static_animation_struct(Static_animation_struct& a)
	: animation(a.animation), frame_time(a.frame_time)
{
	it = animation->cbegin();
}

Frame_info::Frame_info(Vectori part_size_, Vectori frame_size_, Vectori offset_) :
	part_size(part_size_), frame_size(frame_size_), offset(offset_),
	character_position(offset_) {}