#include "logic.h"

void Linear_AI::calc_pos(float dt)
{
	time += dt;
	while (time > it->second)
	{
		time -= it->second;
		it = util::increment_iterator(it, points);
	}
	auto next = util::increment_iterator(it, points);
	if (std::isnan(next->first.x) || std::isnan(next->first.y))
	{
		pos = Vectorf(-INFINITY, -INFINITY);
	}
	else
	{
		float a = time / it->second;
		pos = (1.0f - a) * it->first + a * next->first;
	}
}

sf::Transform Linear_AI::get_pos()
{
	return sf::Transform().translate(pos);
}

void Linear_AI::reset()
{
	time = time_offset;
	it = points.cbegin();
	calc_pos(0);
}

Linear_AI::Linear_AI(std::vector<std::pair<Vectorf, float>> points_,
	float time_offset_) : points(points_), time(time_offset_),
	time_offset(time_offset_)
{
	it = points.cbegin();
	calc_pos(0);
}

void Swing_AI::calc_pos(float dt)
{
	time += dt;
	while (time > 1.0f)
	{
		const float angleAccel = (GRAVITY / line_len) * sin(rad_angle);
		a_speed += angleAccel;
		rad_angle += a_speed;
		time -= 1.0f;

	}
	pos = sf::Vector2f(-sin(rad_angle), cos(rad_angle)) * line_len * context.global_scale;
}

sf::Transform Swing_AI::get_pos()
{
	return sf::Transform().translate(pos);
}

void Swing_AI::reset()
{
	time = 0;
	rad_angle = start_angle;
}

Swing_AI::Swing_AI(const float line_len_, float angle_) : line_len(line_len_),
rad_angle(angle_), start_angle(angle_) {}

void Swing_rotation_AI::calc_pos(float dt)
{
	time += dt;
	while (time > 1.0f)
	{
		const float angleAccel = (GRAVITY / line_len) * sin(rad_angle);
		a_speed += angleAccel;
		rad_angle += a_speed;
		time -= 1.0f;

	}
	pos = sf::Vector2f(cos(rad_angle), sin(rad_angle));
}

sf::Transform Swing_rotation_AI::get_pos()
{
	return 	sf::Transform(
		pos.x, -pos.y, pivot.x * (1 - pos.x) + pivot.y * pos.y,
		pos.y, pos.x, pivot.y * (1 - pos.x) - pivot.x * pos.y,
		0, 0, 1);
}

void Swing_rotation_AI::reset()
{
	time = 0;
	rad_angle = start_angle;
}

Swing_rotation_AI::Swing_rotation_AI(const float line_len_, float angle_, Vectorf pivot_)
	: line_len(line_len_), rad_angle(angle_), pivot(pivot_), start_angle(angle_)
{
}

Accelerated_linear_AI::Accelerated_linear_AI(
	std::vector<std::tuple<Vectorf, float, float>> points_, float time_offset_) :
	points(points_), time(time_offset_), time_offset(time_offset_)
{
	it = points.cbegin();
	calc_pos(0);
}

sf::Transform Accelerated_linear_AI::get_pos()
{
	return sf::Transform().translate(pos);
}

void Accelerated_linear_AI::reset()
{
	time = time_offset;
	it = points.cbegin();
	calc_pos(0);
}

void Accelerated_linear_AI::calc_pos(float dt)
{
	time += dt;
	bool lag = false;
	while (time > std::get<1>(*it))
	{
		time -= std::get<1>(*it);
		it = util::increment_iterator(it, points);
		lag = true;
	}
	if (lag)
	{
		auto [target_pos, t, acc] = *it;
		auto next = util::increment_iterator(it, points);
		Vectorf pos_next = std::get<0>(*next);
		Vectorf diff = pos_next - target_pos;
		s0 = std::hypotf(diff.x, diff.y);
		v0 = s0 / t - acc * t / 2;
	}
	auto [target_pos, delta, acc] = *it;
	auto next = util::increment_iterator(it, points);
	if (std::isnan(std::get<0>(*next).x) || std::isnan(std::get<0>(*next).y))
	{
		pos = Vectorf(-INFINITY, -INFINITY);
	}
	else
	{
		Vectorf pos_next = std::get<0>(*next);
		float s = (v0 + acc * time / 2) * time;
		float a = s / s0;
		pos = (1.0f - a) * target_pos + a * pos_next;
	}
}

void Rotation_AI::calc_pos(float dt)
{
	time += dt;
	while (time > it->second)
	{
		time -= it->second;
		it = util::increment_iterator(it, angles);
	}
	auto next = util::increment_iterator(it, angles);
	float a = time / it->second;
	current_angle = (1.0f - a) * it->first + a * next->first;
	pos = { cos(current_angle),sin(current_angle) };
}

sf::Transform Rotation_AI::get_pos()
{
	return 	sf::Transform(
		pos.x, -pos.y, pivot.x * (1 - pos.x) + pivot.y * pos.y,
		pos.y, pos.x, pivot.y * (1 - pos.x) - pivot.x * pos.y,
		0, 0, 1);
}

void Rotation_AI::reset()
{
	time = time_offset;
	it = angles.cbegin();
	calc_pos(0);
}

Rotation_AI::Rotation_AI(Vectorf pivot_,
	std::vector<std::pair<float, float>> angles_, float time_offset_) :
	time(time_offset), pivot(pivot_), angles(angles_), time_offset(time_offset_)
{
	it = angles.cbegin();
	calc_pos(0);
}

Container_AI::Container_AI(std::vector<std::unique_ptr<Simple_AI>>&& vec_) : ais(std::move(vec_))
{

}

void Container_AI::calc_pos(float dt)
{
	transform = sf::Transform::Identity;
	for (auto& it : ais)
	{
		it->calc_pos(dt);
		transform = it->get_pos() * transform;
	}
}

sf::Transform Container_AI::get_pos()
{
	return transform;
}

void Container_AI::reset()
{
	for (auto& it : ais)
	{
		it.reset();
	}
}
