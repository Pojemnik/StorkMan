#include "animations.h"

Animation_tree::Animation_tree(int count_, int i_count, Frame_info info) :
	count(count_), independent_count(i_count), frame_info(info)
{
	root = 0;
	tree.resize(count);
	position_of_element_in_animation_array.resize(count);
	nodes.resize(count);
}

Vectorf Key_frame_animation::count_pos(Vectorf start, Vectori translation1,
	float a1, Vectori translation2, float a2)
{
	const float size1 = float(tree.frame_info.part_size.x);
	const float size2 = float(tree.frame_info.part_size.y);
	float angle1 = util::deg_to_rad(a1);
	float angle2 = util::deg_to_rad(a2);
	Vectorf l1 = { translation1.x - size1 / 2, translation1.y - size1 / 2 };
	Vectorf l2 = { translation2.x - size2 / 2, translation2.y - size2 / 2 };
	l1 = util::rotate_vector(l1, angle1);
	l2 = util::rotate_vector(l2, angle2);
	return Vectorf(start.x + l1.x - l2.x + size1 / 2 - size2 / 2,
		start.y + l1.y - l2.y + size1 / 2 - size2 / 2);
}

void Key_frame_animation::animate(std::vector<float> frame)
{
	std::vector<util::xyr> vec(tree.count);
	vec[tree.root].pos.x = frame[0];
	vec[tree.root].pos.y = frame[1];
	vec[tree.root].r = util::ang_reduce(frame[2]);
	parts_sprites[tree.root].setRotation(vec[tree.root].r);
	parts_sprites[tree.root].setPosition(vec[tree.root].pos);
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
			parts_sprites[next].setRotation(vec[next].r);
			parts_sprites[next].setPosition(vec[next].pos);
		}
	}
}

void Key_frame_animation::pre_draw()
{
	tex.clear(sf::Color(0, 0, 0, 0));
	for (int i = 0; i < tree.nodes.size(); i++)
	{
		parts_sprites[i].setTexture(*parts[i]->get_texture());
		tex.draw(parts_sprites[i]);
	}
	tex.display();
}

Key_frame_animation::Key_frame_animation(std::vector<std::unique_ptr<Animation_part>>&& parts_,
	std::vector<const Dynamic_animation_struct*> animations_, const Animation_tree& tree_)
	: animations(animations_), tree(tree_), key(0), parts(std::move(parts_)),
	animation(Animation_index::DEFAULT)
{
	last_key = &animations[static_cast<int>(animation)]->key_frames[key];
	time_to_next_frame = animations[static_cast<int>(animation)]->lengths[key];
	key++;
	next_key = &animations[static_cast<int>(animation)]->key_frames[key];
	actual_frame = *last_key;
	for (int i = 0; i < parts.size(); i++)
	{
		Vectori tex_size=parts[i]->get_texture_size();
		parts_sprites.push_back(sf::Sprite(*parts[i]->get_texture()));
		parts_sprites[i].setOrigin((float)tex_size.x / 2, (float)tex_size.y / 2);
	}
	if (!tex.create(tree.frame_info.frame_size.x, tree.frame_info.frame_size.y))
		return;
	next_frame(.0f);
}

void Key_frame_animation::set_animation(Animation_index a)
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

Animation_index Key_frame_animation::get_current_animation() const
{
	return animation;
}

Frame_info Key_frame_animation::get_frame_info() const
{
	Frame_info info = tree.frame_info;
	info.character_position = { actual_frame[0], actual_frame[1] };
	return info;
}

void Key_frame_animation::increment_key()
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

void Key_frame_animation::next_frame(float dt)
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
	for(auto& it : parts)
	{
		it->advance(dt);
	}
	animate(actual_frame);
	pre_draw();
}

const sf::Texture* const Key_frame_animation::get_texture()
{
	return &tex.getTexture();
}

void Key_frame_animation::change_textures_set(int set)
{
	for(auto& it: parts)
	{
		it->set_image(set);
	}
}

Static_animation::Static_animation(Static_animation_struct& animation_, float time_offset) :
	part(animation_, time_offset) {}

Frame_info Static_animation::get_frame_info() const
{
	return part.frame_info;
}

void Static_animation::next_frame(float dt)
{
	part.advance(dt);
}

const sf::Texture* const Static_animation::get_texture()
{
	return part.get_texture();
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

void Static_animation::change_textures_set(int set)
{
	part.set_image(set);
}

One_frame_animation::One_frame_animation(const sf::Texture* _tex) : tex(_tex) {}

void One_frame_animation::next_frame(float dt){}

const sf::Texture* const One_frame_animation::get_texture()
{
	return tex;
}

void One_frame_animation::set_animation(Animation_index a)
{
	throw std::logic_error("Not implemented");
}

Animation_index One_frame_animation::get_current_animation() const
{
	return Animation_index::DEFAULT;
}

Frame_info One_frame_animation::get_frame_info() const
{
	return Frame_info(Vectori(0, 0), static_cast<Vectori>(tex->getSize()), Vectori(0, 0));
}
void One_frame_animation::change_textures_set(int set)
{
}