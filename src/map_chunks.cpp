#include "map_chunks.h"

void Map_chunk::draw_border(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(border, states);
}

void Map_chunk::draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : collidables)
	{
		it->draw_dynamic_collision(target, states);
	}
}

void Map_chunk::draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(static_collision_vertices, states);
}

void Map_chunk::draw_zones(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : zones)
	{

		it->draw(target, states);
	}
}

void Map_chunk::resolve_collisions(Entity& entity) const
{
	entity.resolve_collision(collidables);
}

void Map_chunk::make_zones_interactions(Entity& entity) const
{
	for (auto& it : zones)
	{

		it->interact(entity);
	}
}

Map_chunk::Map_chunk(std::vector<std::shared_ptr<Updatable>>&& updatables_,
	std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
	std::vector<std::shared_ptr<const Collidable>>&& collidables_,
	std::vector<std::shared_ptr<Zone>>&& zones_, sf::FloatRect bound_,
	sf::VertexBuffer&& static_vertices)
	: updatables(updatables_), collidables(collidables_), bound(bound_),
	zones(zones_), static_collision_vertices(static_vertices)
{
	static util::Color_generator colors("data/colors.txt");
	for (auto& it : drawables_)
	{
		layers[it.first].push_back(it.second);
	}
	border.setPosition(bound.left, bound.top);
	border.setSize({ bound.width, bound.height });
	border.setOutlineColor(colors.get_color());
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineThickness(1);
}

void Map_chunk::update(float dt)
{
	for (auto& it : updatables)
	{
		it->update(dt);
	}
}

void Map_chunk::draw_layer(sf::RenderTarget& target, sf::RenderStates states, int layer) const
{
	for (const auto& it : layers[layer])
	{
		target.draw(*it, states);
	}
}

sf::FloatRect Map_chunk::get_bounding_rect() const
{
	return bound;
}