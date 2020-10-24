#include "barriers.h"

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

Moving_barrier::Moving_barrier(std::vector<sf::Vertex>&& vertices_,
	std::unique_ptr<Simple_AI> ai_, Vectorf pos_, Surface_type surface_) :
	Barrier(std::move(vertices_), { 0,0 }, surface_), ai(std::move(ai_)),
	vertex(sf::LineStrip, sf::VertexBuffer::Static), pos(pos_)
{
	base_rect = collision.rect;
	base_mesh = collision.mesh;
	collision.rect.left += pos.x;
	collision.rect.top += pos.y;
	vertex.create(base_mesh.size() + 1);
	sf::Vertex* tmp = new sf::Vertex[base_mesh.size() + 1];
	for (int i = 0; i < base_mesh.size(); i++)
	{
		collision.mesh[i] += pos;
		tmp[i] = sf::Vertex(base_mesh[i], sf::Color::White);
	}
	tmp[base_mesh.size()] = sf::Vertex(base_mesh[0], sf::Color::White);
	vertex.update(tmp);
}

void Moving_barrier::draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(pos);
	states.transform *= ai->get_pos();
	target.draw(vertex, states);
}

Vectorf Moving_barrier::get_speed() const
{
	return speed;
}

void Moving_barrier::update(float dt)
{
	ai->calc_pos(dt);
	sf::Transform new_pos = sf::Transform().translate(pos)*ai->get_pos();
	Vectorf old_pos = { collision.rect.left, collision.rect.top };
	for (int i = 0; i < base_mesh.size(); i++)
	{
		collision.mesh[i] = new_pos * base_mesh[i];
	}
	collision.rect = new_pos.transformRect(base_rect);
	collision.calculate_min_max_arr();
	speed = (Vectorf(collision.rect.left, collision.rect.top) - old_pos) / dt;
}