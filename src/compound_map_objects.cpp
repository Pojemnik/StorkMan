#include "compound_map_objects.h"

const Collision* const Pendulum::get_collision() const
{
	return platform.get_collision();
}

Pendulum::Pendulum(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex> points_, std::vector<Vectorf> attach_pts,
	float angle_, float line_len_, const sf::Texture* const line_texture_,
	Vectori line_size, Surface_type surface_) : 
	platform({ pos_.x,pos_.y - line_len_ * context.global_scale }, texture_, points_,
		std::move(std::make_unique<Swing_AI>(line_len_, angle_)), surface_)
{
	float line_diff = line_len_ * context.global_scale / line_texture_->getSize().y;
	for (const auto& it : attach_pts)
	{
		Swing_rotation_AI* ai = new Swing_rotation_AI(line_len_, angle_, static_cast<Vectorf>(line_size) * line_diff);
		lines.emplace_back(Vectorf(it.x - line_size.x * line_diff, it.y), line_texture_, line_len_,
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
	sf::FloatRect bound = platform.get_collision()->rect;
	for (const auto& it : lines)
	{
		sf::FloatRect object_rect = it.get_bounding_rect();
		bound = util::merge_bounds(bound, object_rect);
	}
	return bound;
}

void Pendulum::update_physics(float dt)
{
	platform.update_physics(dt);
}

void Pendulum::update_graphics(float dt)
{
	for (auto& it : lines)
	{
		it.update_graphics(dt);
	}
}

Vectorf Pendulum::get_speed() const
{
	return platform.get_speed();
}