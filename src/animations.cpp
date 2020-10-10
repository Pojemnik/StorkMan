#include "animations.h"

Animation_tree::Animation_tree(int _count, int i_count, Frame_info info) :
	count(_count), independent_count(i_count), frame_info(info)
{
	root = 0;
	tree.resize(count);
	position_of_element_in_animation_array.resize(count);
	nodes.resize(count);
}

Vectorf Dynamic_animation::count_pos(Vectorf start, Vectori translation1,
	float a1, Vectori translation2, float a2)
{
	const float size1 = tree.frame_info.part_size.x;
	const float size2 = tree.frame_info.part_size.y;
	float angle1 = util::deg_to_rad(a1);
	float angle2 = util::deg_to_rad(a2);
	Vectorf l1 = { translation1.x - size1 / 2, translation1.y - size1 / 2 };
	Vectorf l2 = { translation2.x - size2 / 2, translation2.y - size2 / 2 };
	l1 = util::rotate_vector(l1, angle1);
	l2 = util::rotate_vector(l2, angle2);
	return Vectorf(start.x + l1.x - l2.x + size1 / 2 - size2 / 2,
		start.y + l1.y - l2.y + size1 / 2 - size2 / 2);
}

void Dynamic_animation::animate(std::vector<float> frame)
{
	std::vector<util::xyr> vec(tree.count);
	vec[tree.root].pos.x = frame[0];
	vec[tree.root].pos.y = frame[1];
	vec[tree.root].r = util::ang_reduce(frame[2]);
	parts[tree.root].setRotation(vec[tree.root].r);
	parts[tree.root].setPosition(vec[tree.root].pos);
	std::queue<int> q;
	q.push(tree.root);
	while (!q.empty())
	{
		int current = q.front();
		q.pop();
		for (int i = 0; i < tree.tree[current].size(); i++)
		{
			int next = tree.tree[current][i];
			q.push(next);
			vec[next].r = util::ang_reduce(vec[current].r
				+ frame[tree.position_of_element_in_animation_array[next] + 3]);
			vec[next].pos = count_pos(vec[current].pos,
				tree.nodes[next].delta_pos[0], vec[current].r,
				tree.nodes[next].delta_pos[1], vec[next].r);
			parts[next].setRotation(vec[next].r);
			parts[next].setPosition(vec[next].pos);
		}
	}
}

void Dynamic_animation::pre_draw()
{
	tex.clear(sf::Color(0, 0, 0, 0));
	for (int i = 0; i < tree.nodes.size(); i++)
	{
		tex.draw(parts[i]);
	}
	tex.display();
}

Dynamic_animation::Dynamic_animation(const sf::Texture* texture_,
	std::vector<sf::IntRect>& part_sizes,
	std::vector<const Dynamic_animation_struct*> animations_, const Animation_tree& tree_)
	: animations(animations_), tree(tree_), key(0),
	animation(Animation_index::DEFAULT)
{
	last_key = &animations[static_cast<int>(animation)]->key_frames[key];
	time_to_next_frame = animations[static_cast<int>(animation)]->lengths[key];
	key++;
	next_key = &animations[static_cast<int>(animation)]->key_frames[key];
	actual_frame = *last_key;
	for (int i = 0; i < part_sizes.size(); i++)
	{
		parts.push_back(sf::Sprite(*texture_, part_sizes[i]));
		parts[i].setOrigin((float)part_sizes[i].width / 2, (float)part_sizes[i].height / 2);
	}
	if (!tex.create(tree.frame_info.frame_size.x, tree.frame_info.frame_size.y))
		return;
	next_frame(.0f);
}

void Dynamic_animation::set_animation(Animation_index a)
{
	Animation_index alternative = Animation_index::DEFAULT;
	std::pair<int, int> transition = std::make_pair(key, static_cast<int>(a));
	if (tree.alternative_animations.contains(transition))
	{
		bool contains = false;
		for (const auto& it : tree.alternative_animations.at(transition).second)
		{
			if (it == key)
			{
				contains = true;
				break;
			}
		}
		if (contains)
		{
			alternative = tree.alternative_animations.at(transition).first;
		}
	}
	key = 0;
	time_to_next_frame = ANIMATION_CHANGE_DELTA;
	if (alternative == Animation_index::DEFAULT)
	{
		animation = a;
	}
	else
	{
		animation = alternative;
	}
	last_key = next_key;
	next_key = &animations[static_cast<int>(animation)]->key_frames[0];
}

Animation_index Dynamic_animation::get_current_animation() const
{
	return animation;
}

Frame_info Dynamic_animation::get_frame_info() const
{
	Frame_info info = tree.frame_info;
	info.character_position = { actual_frame[0], actual_frame[1] };
	return info;
}

void Dynamic_animation::increment_key()
{
	last_key = next_key;
	actual_frame = *last_key;
	if (++key >= animations[static_cast<int>(animation)]->key_frames.size())
	{
		if (animations[static_cast<int>(animation)]->repeat)
		{
			time_to_next_frame += animations[static_cast<int>(animation)]->lengths.back();
			key = 0;
		}
		else
		{
			set_animation(Animation_index::DEFAULT);
		}
	}
	else
	{
		time_to_next_frame += animations[static_cast<int>(animation)]->lengths[key];
	}
	next_key = &animations[static_cast<int>(animation)]->key_frames[key];
}

void Dynamic_animation::next_frame(float dt)
{
	time_to_next_frame -= dt;
	while (time_to_next_frame <= 0)
	{
		increment_key();
	}
	float alfa = time_to_next_frame / animations[static_cast<int>(animation)]->lengths[key];
	for (int i = 0; i < tree.independent_count + 3; i++)
	{
		actual_frame[i] = (*last_key)[i] * alfa + (*next_key)[i] * (1.f - alfa);
	}
	animate(actual_frame);
	pre_draw();
}

const sf::Texture* const Dynamic_animation::get_texture()
{
	return &tex.getTexture();
}

Dynamic_animation_struct::Dynamic_animation_struct(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r)
	: key_frames(kf), lengths(l), repeat(r) {}

Static_animation_struct::Static_animation_struct(const std::vector<const sf::Texture*>* animation_, float frame_time_) :
	animation(animation_), frame_time(frame_time_)
{
	it = animation->cbegin();
}

Static_animation_struct::Static_animation_struct(Static_animation_struct& a) : animation(a.animation), frame_time(a.frame_time)
{
	it = animation->cbegin();
}

Static_animation::Static_animation(Static_animation_struct& animation_, float time_offset) :
	animation(animation_), time(time_offset), frame_info(Vectori((*animation_.it)->getSize()),
		Vectori((*animation_.it)->getSize()), { 0,0 })
{
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
	next_frame(.0f);
}

void Static_animation::next_frame(float dt)
{
	time += dt;
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
}

const sf::Texture* const Static_animation::get_texture()
{
	return *animation.it;
}

void Static_animation::set_animation(Animation_index a)
{
	//This should never happen
	//If changed, change also get_current_animation
	throw std::logic_error("Not implemented");
}

Animation_index Static_animation::get_current_animation() const
{
	//This should never happen
	throw std::logic_error("Not implemented");
	return Animation_index::DEFAULT;
}

Frame_info Static_animation::get_frame_info() const
{
	return frame_info;
}

Frame_info::Frame_info(Vectori part_size_, Vectori frame_size_, Vectori offset_) :
	part_size(part_size_), frame_size(frame_size_), offset(offset_), 
	character_position(offset_) {}
