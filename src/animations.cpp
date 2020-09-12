#include "animations.h"

Vectorf Dynamic_animation::count_pos(Vectorf start, const float size1, const float size2,
	Vectori translation1, float a1, Vectori translation2, float a2)
{
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
			vec[next].pos = count_pos(vec[current].pos, 128, 128,
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

Dynamic_animation::Dynamic_animation(sf::Texture* texture_,
	std::vector<sf::IntRect>& part_sizes,
	std::vector<const Dynamic_animation_struct*> animations_, Animation_tree tree_)
	: animations(animations_), tree(tree_)
{
	last_animation_n = animation_n = 0;
	key = 0;
	last_key = &animations[animation_n]->key_frames[key];
	time_to_next_frame = animations[animation_n]->lengths[key];
	key++;
	next_key = &animations[animation_n]->key_frames[key];
	actual_frame = *last_key;
	for (int i = 0; i < part_sizes.size(); i++)
	{
		parts.push_back(sf::Sprite(*texture_, part_sizes[i]));
		parts[i].setOrigin((float)part_sizes[i].width / 2, (float)part_sizes[i].height / 2);
	}
	if (!tex.create(500, 500))
		return;
}

void Dynamic_animation::set_animation(int val)
{
	animation_n = val;
	time_to_next_frame += ANIMATION_CHANGE_DELTA;
	key = 0;
	next_key = &animations[animation_n]->key_frames[0];
}

void Dynamic_animation::increment_key()
{
	last_key = next_key;
	if (++key >= animations[animation_n]->key_frames.size())
	{
		if (animations[animation_n]->repeat)
		{
			time_to_next_frame += animations[animation_n]->lengths.back();
			key = 0;
		}
		else
		{
			animation_n = 0;
			set_animation(animation_n);
		}
	}
	else
	{
		time_to_next_frame += animations[animation_n]->lengths.back();
	}
	next_key = &animations[animation_n]->key_frames[key];
}

const sf::Texture* const Dynamic_animation::next_frame(float dt)
{
	time_to_next_frame -= dt;
	if (last_animation_n != animation_n)
	{
		set_animation(animation_n);
	}
	while (time_to_next_frame < 0)
	{
		increment_key();
	}
	float alfa = time_to_next_frame / animations[animation_n]->lengths[key];
	for (int i = 0; i < tree.independent_count + 3; i++)
	{
		actual_frame[i] = (*last_key)[i] * alfa + (*next_key)[i] * (1.f - alfa);
	}
	animate(actual_frame);
	pre_draw();
	return &tex.getTexture();
}

Dynamic_animation_struct::Dynamic_animation_struct(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r)
	: key_frames(kf), lengths(l), repeat(r) {}

Animation_struct::Animation_struct(const std::vector<sf::Texture>* animation_, float frame_time_) :
	animation(animation_), frame_time(frame_time_)
{
	it = animation->cbegin();
}

Animation_struct::Animation_struct(Animation_struct& a) : animation(a.animation), frame_time(a.frame_time)
{
	it = animation->cbegin();
}

Static_animation::Static_animation(Animation_struct& animation_, float time_offset) :
	animation(animation_), time(time_offset)
{
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
}

const sf::Texture* const Static_animation::next_frame(float dt)
{
	time += dt;
	while (time >= animation.frame_time)
	{
		time -= animation.frame_time;
		animation.it = util::increment_iterator(animation.it, *animation.animation);
	}
	return &*animation.it;
}

void Static_animation::set_animation(int val)
{
	//This should never happen
	throw std::runtime_error("Unimplemented");
}
