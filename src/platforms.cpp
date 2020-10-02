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
	bound = util::mesh_to_rect(points);
}

sf::FloatRect Textured_polygon::get_bounding_rect() const
{
	return bound;
}

void Textured_polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	target.draw(shape, states);
}

const Collision* const Platform::get_collision() const
{
	return &collision;
}

Platform::Platform(Vectorf pos_, const sf::Texture* texture_, std::vector<sf::Vertex> points_) :
	collision(points_, pos_), Textured_polygon(pos_, texture_, std::move(std::vector<sf::Vertex>(points_))) {}

sf::FloatRect Platform::get_bounding_rect() const
{
	return collision.rect;
}
const Collision* const Moving_platform::get_collision() const
{
	return &collision;
}

Moving_platform::Moving_platform(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex>&& points_, std::unique_ptr<Simple_AI> ai_) :
	Platform(pos_, texture_, std::move(std::vector<sf::Vertex>(points_))),
	ai(std::move(ai_))
{
	base_rect = collision.rect;
	base_mesh = collision.mesh;
}

void Moving_platform::update(float dt)
{
	ai->calc_pos(dt);
	sf::Transform new_pos = ai->get_pos();
	for (int i = 0; i < base_mesh.size(); i++)
	{
		collision.mesh[i] = new_pos * base_mesh[i];
	}
	collision.rect = new_pos.transformRect(base_rect);
	collision.calculate_min_max_arr();
}

void Moving_platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	target.draw(shape, states);
}

Barrier::Barrier(std::vector<sf::Vertex>&& vertices_, Vectorf pos_)
	: collision(std::move(vertices_), pos_) {}

const Collision* const Barrier::get_collision() const
{
	return &collision;
}

sf::FloatRect Barrier::get_bounding_rect() const
{
	return collision.rect;
}
