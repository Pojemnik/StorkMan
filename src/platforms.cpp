#include "platforms.h"

Textured_polygon::Textured_polygon(Vectorf pos, const sf::Texture* texture_,
	std::vector<sf::Vertex> points) : texture(texture_)
{
	for (auto& it : points)
	{
		it.position += pos;
	}
	shape = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Static);
	shape.create(points.size());
	shape.update(&points[0]);
}

void Textured_polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	target.draw(shape, states);
}

Platform::Platform(Vectorf pos_, const sf::Texture* texture_, std::vector<sf::Vertex>&& points_) :
	Textured_polygon(pos_, texture_, std::move(points_))
{
	//TODO: Construct rectangle
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
		lines.emplace_back(Vectorf(it.x - line_size.x, it.y), line_texture_, line_len_, std::move(std::unique_ptr<Swing_rotation_AI>(ai)));
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

void Pendulum::update(float dt)
{
	for (auto& it : lines)
	{
		it.update(dt);
	}
	platform.update(dt);
}

Moving_platform::Moving_platform(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex>&& points_, std::unique_ptr<Simple_AI> ai_) :
	Platform(pos_, texture_, std::move(points_)), ai(std::move(ai_)) {}

void Moving_platform::update(float dt)
{
	ai->calc_pos(dt);
}

void Moving_platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	target.draw(shape, states);
}

Barrier::Barrier(std::vector<Vectorf>&& vertices_, Vectorf pos_) : vertices(std::move(vertices_))
{
	for (auto& it : vertices)
	{
		it += pos_;
	}
	//TODO: Construct rectangle
}
