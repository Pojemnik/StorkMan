#include "worldparts.h"

Wall::Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
	int layer)
	: Texturable(p, t, points, layer) {}

Object::Object(Vectorf p, const sf::Texture* t, float h, int layer)
	: Renderable(p, t, h, layer) {}

Object::Object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
	float ang)
	: Renderable(p, t, h, layer, flip, ang) {}

void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(move_pos);
	Renderable::draw(target, states);
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

Moving_object::Moving_object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
	float ang, Linear_move path) : Object(p, t, h, layer, flip, ang), move_data(path) {
	move_data.it = move_data.points.begin();
}

Moving_object::Moving_object(const Moving_object& mo) : Object(mo), move_data(mo.move_data)
{
	move_data.it = move_data.points.begin();
}

Animated_object::Animated_object(Vectorf p,
	const std::vector<sf::Texture>* a, float h, int layer, int fr)
	: Animatable(p, a, h, layer), frames_diff(fr) {}

Animated_object::Animated_object(Vectorf p,
	const std::vector<sf::Texture>* a, float h, int layer, int fr,
	int flip, float ang, int frame_delta)
	: Animatable(p, a, h, layer, flip, ang), frames_diff(fr)
{
	for (int i = 0; i < frame_delta / frames_diff; i++)
	{
		Animatable::next_frame();
	}
	frame_counter = frame_delta % frames_diff;
}

void Animated_object::next_frame()
{
	if (++frame_counter == frames_diff)
	{
		frame_counter = 0;
		Animatable::next_frame();
	}
}

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