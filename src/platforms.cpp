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

Platform::Platform(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex> points_, Surface_type surface_) :
	collision(points_, pos_, surface_),
	Textured_polygon(pos_, texture_, std::move(std::vector<sf::Vertex>(points_))) {}

sf::FloatRect Platform::get_bounding_rect() const
{
	return collision.rect;
}
const Collision* const Moving_platform::get_collision() const
{
	return &collision;
}

Moving_platform::Moving_platform(Vectorf pos_, const sf::Texture* texture_,
	std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
	Surface_type surface_) : Platform(pos_, texture_, points_, surface_), ai(std::move(ai_)),
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

Barrier::Barrier(std::vector<sf::Vertex>&& vertices_, Vectorf pos_, Surface_type surface_)
	: collision(std::move(vertices_), pos_, surface_) {}

const Collision* const Barrier::get_collision() const
{
	return &collision;
}

sf::FloatRect Barrier::get_bounding_rect() const
{
	return collision.rect;
}

Animated_polygon::Animated_polygon(Vectorf pos, std::unique_ptr<Animation>&& animation_,
	std::vector<sf::Vertex> points) : animation(std::move(animation_)),
	Textured_polygon(pos, nullptr, points)
{
	update(.0f);
}

void Animated_polygon::update_frame()
{
	texture = animation->get_texture();
}

void Animated_polygon::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	target.draw(polygon, states);
}

sf::FloatRect Animated_polygon::get_bounding_rect() const
{
	return bound;
}

void Animated_polygon::update(float dt)
{
	next_frame(dt);
	update_frame();
}

Animated_moving_platform::Animated_moving_platform(Vectorf pos_,
	std::unique_ptr<Animation>&& animation_, std::vector<sf::Vertex> points_,
	std::unique_ptr<Simple_AI> ai_, Surface_type surface_)
	: Moving_platform(pos_, nullptr, points_, std::move(ai_), surface_),
	animation(std::move(animation_))
{
	update(.0f);
}

void Animated_moving_platform::update(float dt)
{
	Moving_platform::update(dt);
	next_frame(dt);
	update_frame();
}

void Animated_moving_platform::update_frame()
{
	texture = animation->get_texture();
}

void Animated_moving_platform::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_moving_platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	states.transform *= ai->get_pos();
	target.draw(polygon, states);
}

void Animated_moving_platform::draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	target.draw(vertex, states);
}
