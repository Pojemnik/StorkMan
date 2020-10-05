#include "platforms.h"

Textured_polygon::Textured_polygon(Vectorf pos, const sf::Texture* texture_,
	std::vector<sf::Vertex> points) : texture(texture_)
{
	for (auto& it : points)
	{
		it.position += pos;
	}
	polygon = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Static);
	polygon.create(points.size());
	polygon.update(&points[0]);
	bound = util::mesh_to_rect(points);
}

sf::FloatRect Textured_polygon::get_bounding_rect() const
{
	return bound;
}

void Textured_polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	target.draw(polygon, states);
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
	std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_) :
	Platform(pos_, texture_, points_), ai(std::move(ai_)),
	vertex(sf::LineStrip, sf::VertexBuffer::Static)
{
	base_rect = collision.rect;
	base_mesh = collision.mesh;
	vertex.create(base_mesh.size() + 1);
	sf::Vertex* tmp = new sf::Vertex[base_mesh.size() + 1];
	for (int i = 0; i < base_mesh.size(); i++)
	{
		tmp[i] = sf::Vertex(base_mesh[i], sf::Color::White);
	}
	tmp[base_mesh.size()] = sf::Vertex(base_mesh[0], sf::Color::White);
	vertex.update(tmp);
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
	Platform::draw(target, states);
}

void Moving_platform::draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	target.draw(vertex, states);
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
