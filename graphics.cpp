#include "graphics.h"

Renderable::Renderable(Vectorf p, sf::Texture* t, float h) : tex(t), pos(p), height(h)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Renderable::rescale(float gs)
{
	float scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

Texturable::Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points)
	: tex(t), pos(p), vertices(points)
{
	shape = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Stream);
	shape.create(vertices.size());
	shape.update(&vertices[0]);
}

void Texturable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

Animatable::Animatable(Vectorf p, const Animation* t, float h, float gs)
	: tex(t), pos(p), height(h)
{
	it = tex->begin();
	sprite = sf::Sprite(*it);
	sprite.setPosition(pos);
	scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

void Animatable::next_frame()
{
	if (++it == tex->end())
		it = tex->begin();
	sprite.setTexture(*it);
}

void Animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Animation::Animation(std::vector<sf::Texture>& a, Vectorf c, sf::FloatRect rect_col)
	: content(std::move(a)), center(c), rect_collision(rect_col) {}

std::vector<sf::Texture>::const_iterator Animation::begin() const
{
	return content.begin();
}

std::vector<sf::Texture>::const_iterator Animation::end() const
{
	return content.end();
}

Vectorf Dynamic_animatable::count_pos(Vectorf start, float size1, float size2,
	Vectorf translation1, float a1, Vectorf translation2, float a2)
{
	float angle1 = util::rdn(a1);
	float angle2 = util::rdn(a2);
	float d1 = sqrt(pow(translation1.x - size1 / 2, 2) + pow(translation1.y - size1 / 2, 2));
	float d2 = sqrt(pow(translation2.x - size2 / 2, 2) + pow(translation2.y - size2 / 2, 2));
	float sinalfa = (translation1.y - size1 / 2) / d1;
	float cosalfa = (translation1.x - size1 / 2) / d1;
	float sinbeta = (translation2.y - size2 / 2) / d2;
	float cosbeta = (translation2.x - size2 / 2) / d2;
	Vectorf l1 = { (cosalfa * cos(angle1) - sinalfa * sin(angle1)) * d1,
		(sinalfa * cos(angle1) + cosalfa * sin(angle1)) * d1 };
	Vectorf l2 = { (cosbeta * cos(angle2) - sinbeta * sin(angle2)) * d2,
		(sinbeta * cos(angle2) + cosbeta * sin(angle2)) * d2 };
	return Vectorf(start.x + l1.x - l2.x + size1 / 2 - size2 / 2,
		start.y + l1.y - l2.y + size1 / 2 - size2 / 2);
}

void Dynamic_animatable::animate(std::vector<float> frame)
{
	std::vector<util::xyr> vec(tree.count);
	vec[tree.root].pos.x = frame[0];
	vec[tree.root].pos.y = frame[1];
	vec[tree.root].r = util::ang_reduce(frame[2]);
	parts[tree.root].setRotation(vec[tree.root].r);
	parts[tree.root].setPosition(vec[tree.root].pos);
	std::queue<int> q;
	q.push(tree.root);
	maxy = 0;
	miny = 500;
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
			if (vec[next].pos.y < miny)
			{
				miny = vec[next].pos.y;
			}
			else
			{
				if (vec[next].pos.y > maxy)
					maxy = vec[next].pos.y;
			}
		}
		tex.clear(sf::Color(0, 0, 0, 0));
		for (int i = 0; i < tree.nodes.size(); i++)
		{
			tex.draw(parts[i]);
		}
		tex.display();
		sprite.setTexture(tex.getTexture());
		sf::FloatRect rect = sprite.getGlobalBounds();
		sprite.setScale(scale, fabs(scale));
	}
}

Dynamic_animatable::Dynamic_animatable(sf::Texture* texture, std::vector<sf::IntRect>& v,
	Vectorf p, std::vector<const Dynamic_animation*> a, Animation_tree t, float h, float gs)
	: pos(p), height(h), animations(a), tree(t)
{
	last_animation_status = animation_status = Animation_status::A_IDLE;
	key = 0;
	last_key = &animations[animation_status]->key_frames[key];
	frames_delta = animations[animation_status]->lengths[key];
	next_key = &animations[animation_status]->key_frames[++key];
	actual_frame = *last_key;
	for (int i = 0; i < v.size(); i++)
	{
		parts.push_back(sf::Sprite(*texture, v[i]));
		parts[i].setOrigin(v[i].width / 2, v[i].height / 2);
	}
	scale = gs * height / 350;
	if (!tex.create(500, 500))
		return;
}

void Dynamic_animatable::set_animation(Animation_status s)
{
	animation_status = s;
	frames_delta = ANIMATION_CHANGE_DELTA;
	key = 0;
	next_key = &animations[animation_status]->key_frames[0];
}

void Dynamic_animatable::next_frame()
{
	if (last_animation_status != animation_status)
	{
		set_animation(animation_status);
	}
	if (frames_delta > 1)
	{
		for (int i = 0; i < tree.independent_count + 3; i++)
		{
			float a1 = (*next_key)[i] - actual_frame[i];
			actual_frame[i] = actual_frame[i]
				+ a1 / frames_delta;
		}
		frames_delta--;
	}
	else
	{
		last_key = next_key;
		actual_frame = *last_key;
		if (++key >= animations[animation_status]->key_frames.size())
		{
			if (animations[animation_status]->repeat)
			{
				frames_delta = animations[animation_status]->lengths.back();//Animation loop
				key = 0;
			}
			else
			{
				animation_status = Animation_status::A_IDLE;
				set_animation(animation_status);
			}
		}
		else
		{
			frames_delta = animations[animation_status]->lengths[key];
		}
		next_key = &animations[animation_status]->key_frames[key];
	}
	animate(actual_frame);
}

void Dynamic_animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Dynamic_animation::Dynamic_animation(std::vector<std::vector<float>>& kf, std::vector<int>& l, bool r)
	: key_frames(kf), lengths(l), repeat(r)
{
}
