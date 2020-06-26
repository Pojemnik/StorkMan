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

Pendulum::Pendulum(sf::Texture* pen_tex, sf::Texture* line_tex_,
	std::vector<Vectorf> attach, std::vector<sf::Vertex> points_,
	float line_l, Vectorf pos_, float velocity_, int layer_)
	: Moving_platform(pen_tex, { pos_.x, pos_.y + line_l * context.global_scale },
		points_, layer_), line_len(line_l), line_tex(line_tex_)
{
	speed = { velocity_,0 };
	pos = pos_;
	tex = pen_tex;
	for (auto& i : attach)
	{
		sf::Sprite s(*line_tex);
		Vectorf l_p = { i.x - 250, i.y };
		scale = context.global_scale * line_len / 445.f;
		s.setScale(1, scale);
		lines.push_back({ s,l_p });
	}
	anchor = pos;
	anchor.y -= line_len*context.global_scale;
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
	const float angleAccel = (-0.02 / line_len) * sin(a);
	a_speed += angleAccel * dt;
	a += a_speed * dt;

	angle = { cos(a),sin(a) };
	update_position(dt);
}

void Pendulum::move(Vectorf delta)
{
	//?
}

void Pendulum::update_position(float dt)
{
	pos = anchor + Vectorf(-angle.y,angle.x) *line_len*context.global_scale;
}

Moving_platform::Moving_platform(sf::Texture* tex, Vectorf p,
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
