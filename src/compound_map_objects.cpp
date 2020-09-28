#include "compound_map_objects.h"

const Collision* const Pendulum::get_collision() const
{
	return platform.get_collision();
}

Pendulum::Pendulum(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex>&& points_, std::vector<Vectorf> attach_pts,
	float angle_, float line_len_, const sf::Texture* const line_texture_,
	Vectori line_size) : platform(pos_, texture_, std::move(points_),
		std::move(std::make_unique<Swing_AI>(line_len_, angle_)))
{
	for (const auto& it : attach_pts)
	{
		Swing_rotation_AI* ai = new Swing_rotation_AI(line_len_, angle_, it);
		lines.emplace_back(Vectorf(it.x - line_size.x, it.y), line_texture_, line_len_,
			std::move(std::unique_ptr<Swing_rotation_AI>(ai)));
	}
}

void Pendulum::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : lines)
	{
		it.draw(target, states);
	}
	platform.draw(target, states);
}

sf::FloatRect Pendulum::get_bounding_rect() const
{
	return platform.get_collision()->rect;
}

void Pendulum::update(float dt)
{
	for (auto& it : lines)
	{
		it.update(dt);
	}
	platform.update(dt);
}
