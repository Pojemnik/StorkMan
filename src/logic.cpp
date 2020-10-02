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
	float a = time / it->second;
	pos = (1.0f - a) * it->first + a * next->first;
}

sf::Transform Linear_AI::get_pos()
{
	return sf::Transform().translate(pos);
}

Linear_AI::Linear_AI(std::vector<std::pair<Vectorf, float>> points_, float time_offset): points(points_),time(time_offset)
{
	it = points.cbegin();
	calc_pos(0);
}

void Swing_AI::calc_pos(float dt)
{
	const float angleAccel = (GRAVITY / line_len) * sin(rad_angle);
	a_speed += angleAccel * dt;
	rad_angle += a_speed * dt;
	pos=sf::Vector2f(-sin(rad_angle),cos(rad_angle))*line_len*context.global_scale;
}

sf::Transform Swing_AI::get_pos()
{
	return sf::Transform().translate(pos);
}

Swing_AI::Swing_AI(const float line_len_, float angle_): line_len(line_len_), rad_angle(angle_)
{
}

void Swing_rotation_AI::calc_pos(float dt)
{
	const float angleAccel = (GRAVITY / line_len) * sin(rad_angle);
	a_speed += angleAccel * dt;
	rad_angle += a_speed * dt;
	pos = sf::Vector2f(cos(rad_angle), sin(rad_angle));
}

sf::Transform Swing_rotation_AI::get_pos()
{
	return 	sf::Transform(
		pos.x, -pos.y, pivot.x * (1 - pos.x) + pivot.y * pos.y,
		pos.y, pos.x, pivot.y * (1 - pos.x) - pivot.x * pos.y,
		0, 0, 1);
}

Swing_rotation_AI::Swing_rotation_AI(const float line_len_, float angle_, Vectorf pivot_)
	: line_len(line_len_), rad_angle(angle_), pivot(pivot_)
{
}
