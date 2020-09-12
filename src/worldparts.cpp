#include "worldparts.h"

const Vectorf fliptab[] = { {1,1},{-1,1},{1,-1},{-1,-1} };

Object::Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
	int flip_, float angle_) : pos(pos_), height(height_), flip(flip_), sprite(*texture_)
{
	sprite.setScale(fliptab[flip] * height);
	sprite.setRotation(angle_);
	sprite.setPosition(pos * context.global_scale);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Object::rescale(float ratio)
{
	float new_scale = ratio * context.global_scale * height / sprite.getTexture()->getSize().y;
	pos *= ratio;
	sprite.setPosition(pos);
	sprite.setScale(fliptab[flip] * new_scale);
}

Moving_object::Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
	Linear_move path, int flip_, float angle_) :
	Object(pos_, texture_, height_, flip_, angle_), move_data(path) {}

Moving_object::Moving_object(const Moving_object& mo) : Object(mo),
move_data(mo.move_data) {}

void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(move_pos);
	Object::draw(target, states);
}

void Moving_object::update(float dt)
{
	time += dt;
	while (time > move_data.it->second)
	{
		time -= move_data.it->second;
		move_data.it = util::increment_iterator(move_data.it, move_data.points);
	}
	auto next = util::increment_iterator(move_data.it, move_data.points);
	float a = time / move_data.it->second;
	move_pos = (1.0f - a) * move_data.it->first + a * next->first;
}

void Moving_object::rescale(float ratio)
{
	Object::rescale(ratio);
	move_data.rescale(ratio);
}

Animated_object::Animated_object(Vectorf pos_, const std::vector<sf::Texture>* animation_,
	float height_, int frame_time_,int flip_, float angle_, float time_offset_) :
	Object(pos_, &(*animation_)[0], height_, flip_, angle_), time(time_offset_),
	animation(animation_), frame_time(frame_time_)
{
	it = animation->cbegin();
}

void Animated_object::next_frame(float dt)
{
	time += dt;
	while (time >= frame_time)
	{
		time -= frame_time;
		it = util::increment_iterator(it, *animation);
	}
	sprite.setTexture(*it);
}

Moving_animated_object::Moving_animated_object(Vectorf pos_,
	const std::vector<sf::Texture>* animation_, float height_, int frame_time_,
	Linear_move path, int flip_, float angle_, float time_offset_) :
	Animated_object(pos_, animation_, height_, frame_time_, flip_, angle_,
		time_offset_), move_data(path) {}

void Moving_animated_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(move_pos);
	Object::draw(target, states);
}

void Moving_animated_object::update(float dt)
{
	time += dt;
	while (time > move_data.it->second)
	{
		time -= move_data.it->second;
		move_data.it = util::increment_iterator(move_data.it, move_data.points);
	}
	auto next = util::increment_iterator(move_data.it, move_data.points);
	float a = time / move_data.it->second;
	move_pos = (1.0f - a) * move_data.it->first + a * next->first;
}

Wall::Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
	int layer)
	: Texturable(p, t, points, layer) {}

Zone::Zone(const std::vector<Vectorf>& vert, Vectorf p) : vertices(vert), pos(p)
{
	static int next_id(0);
	id = next_id++;
	center = { 0,0 };
	max_x = -INFINITY;
	for (auto it : vertices)
	{
		if (it.x > max_x)
			max_x = it.x;
		center += it;
	}
	max_x += pos.x;
	center = { center.x / vertices.size(), center.y / vertices.size() };
}

Zone::Zone(std::vector<Vectorf>& vert, Vectorf p) : Zone((const std::vector<Vectorf>)vert, p)
{}

bool Zone::contains(Vectorf p)
{
	return(util::contained_in_polygon(p - pos, max_x, vertices));
}

Damage_zone::Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
	std::vector<std::pair<int, int>>& dmg) : Zone(vert, p), damage(dmg)
{
	current_damage = damage.begin();
}

Damage_zone::Damage_zone(const Damage_zone& dmgz) : Zone(dmgz.vertices, dmgz.pos),
damage(dmgz.damage)
{
	current_damage = damage.begin();
}

void Damage_zone::update(float dt)
{
	time += dt;
	changed_damage = false;
	while (time >= current_damage->second)
	{
		time -= current_damage->second;
		current_damage = util::increment_iterator(current_damage, damage);
		changed_damage = true;
	}
}