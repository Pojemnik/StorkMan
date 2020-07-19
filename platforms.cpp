#include "platforms.h"
#include <assert.h>
Platform::Platform(Vectorf p, const sf::Texture* t,
	std::vector<sf::Vertex> points, int layer, bool v)
	: Texturable(p, t, points, layer), visible(v)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		mesh.vertices.push_back({ it.position.x + pos.x, it.position.y + pos.y });
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	rect_collision = sf::FloatRect(minx + p.x, miny + p.y, maxx - minx, maxy - miny);
}

void Platform::rescale(float ratio)
{
	Texturable::rescale(ratio);
	Collidable::rescale(ratio);
}

Pendulum::Pendulum(const sf::Texture* pen_tex, const sf::Texture* line_tex_,
	std::vector<Vectorf> attach, std::vector<sf::Vertex> points_,
	float line_l, Vectorf pos_, float angle_, int layer_)
	: Moving_platform(pen_tex, pos_, points_, layer_), line_len(line_l),
	line_tex(line_tex_), rad_angle(angle_)
{
	pos = pos_;
	for (auto& i : attach)
	{
		sf::Sprite s(*line_tex);
		Vectorf l_p = { i.x - 250, i.y };
		scale = context.global_scale * line_len / 445.f;
		s.setScale(1, scale);
		lines.push_back({ s,l_p });
	}
	anchor = pos;
	anchor.y -= line_len * context.global_scale;
}

void Pendulum::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Vectorf point(256, 35 * scale);
	sf::Transform rotate_transf(
		angle.x, -angle.y, point.x * (1 - angle.x) + point.y * angle.y,
		angle.y, angle.x, point.y * (1 - angle.x) - point.x * angle.y,
		0, 0, 1);
	sf::Transform tmp = states.transform;
	sf::Transform tmp2 = states.transform * rotate_transf;
	for (const auto& i : lines)
	{
		states.transform = sf::Transform(1, 0, i.second.x, 0, 1, i.second.y, 0, 0, 1) * tmp2;
		target.draw(i.first, states);
	}
	states.transform = tmp;
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

void Pendulum::update(float dt)
{
	//Pendulum physics
	const float angleAccel = (-0.008 / line_len) * sin(rad_angle);
	a_speed += angleAccel * dt;
	rad_angle += a_speed * dt;
	angle = { cos(rad_angle),sin(rad_angle) };
	update_position(dt);
}

void Pendulum::move(Vectorf delta)
{
	//?
}

void Pendulum::update_position(float dt)
{
	Vectorf delta = pos;
	pos = anchor + Vectorf(-angle.y, angle.x) * line_len * context.global_scale;
	delta = pos - delta;
	total_speed = delta;
	for (auto& v : mesh.vertices)
	{
		v += delta;
	}
	rect_collision.left += delta.x;
	rect_collision.top += delta.y;
}

Moving_platform::Moving_platform(const sf::Texture* tex, Vectorf p,
	std::vector<sf::Vertex> pts, int l) : Texturable(p, tex, pts, l)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		mesh.vertices.push_back(it.position + pos);
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	rect_collision = sf::FloatRect(minx + p.x, miny + p.y, maxx - minx, maxy - miny);
	mass = INFINITY;
	type = Collidable_type::GROUND;
}

void Moving_platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}

Linear_moving_platform::Linear_moving_platform(Linear_move path,
	const sf::Texture* tex_, Vectorf pos_, std::vector<sf::Vertex> pts, int layer_)
	: Moving_platform(tex_, pos_, pts, layer_), move_data(path)
{
	move_data.it = move_data.points.begin();
}

void Linear_moving_platform::update(float dt)
{
	time += dt;
	while (time > move_data.it->second)
	{
		time -= move_data.it->second;
			move_data.it = util::increment_iterator(move_data.it, move_data.points);
		//move_data.it++;
		//move_data.it = move_data.it == move_data.points.end() ? move_data.points.begin() : move_data.it;
	}
	update_position(dt);
}

void Linear_moving_platform::update_position(float dt)
{
	Vectorf delta = pos;
	auto next = util::increment_iterator(move_data.it, move_data.points);
	//auto next = move_data.it + 1 == move_data.points.end() ? move_data.points.begin() : move_data.it + 1;
	float a = time / move_data.it->second;
	pos = (1.0f - a) * move_data.it->first + a * next->first;
	delta = pos - delta;
	total_speed = delta;
	for (auto& v : mesh.vertices)
	{
		v += delta;
	}
	rect_collision.left += delta.x;
	rect_collision.top += delta.y;
}

void Linear_moving_platform::move(Vectorf delta)
{

}

void Linear_moving_platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

Linear_moving_platform::Linear_moving_platform(const Linear_moving_platform& lmp)
	: move_data(lmp.move_data), time(lmp.time), Moving_platform(lmp)
{
	move_data.it = move_data.points.begin();
}