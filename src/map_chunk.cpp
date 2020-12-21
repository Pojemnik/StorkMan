#include "map_chunk.h"

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

Map_chunk::Map_chunk(std::vector<std::shared_ptr<Physical_updatable>>&& p_updatables_,
	std::vector<std::shared_ptr<Graphical_updatable>>&& g_updatables_,
	std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
	std::vector<std::shared_ptr<const Collidable>>&& collidables_,
	std::vector<std::shared_ptr<Zone>>&& zones_,
	std::vector<std::shared_ptr<Interactive>>&& interactives_,
	sf::FloatRect bound_, sf::VertexBuffer&& static_vertices)
	: p_updatables(std::move(p_updatables_)),
	g_updatables(std::move(g_updatables_)), collidables(std::move(collidables_)),
	bound(bound_), zones(std::move(zones_)),
	static_collision_vertices(std::move(static_vertices)),
	interactives(std::move(interactives_))
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

void Map_chunk::update_graphics(float dt)
{
	for (auto& it : g_updatables)
	{
		it->update_graphics(dt);
	}
}

void Map_chunk::update_physics(float dt, std::vector<int>& msg_up)
{
	for (auto& it : p_updatables)
	{
		it->update_physics(dt);
	}
	for (auto& it : interactives)
	{
		std::vector<int> events = it->get_events();
		msg_up.insert(msg_up.begin(), events.begin(), events.end());
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

std::pair<std::vector<Vectorf>, std::vector<std::pair<Vectorf, Vectorf>>> Map_chunk::get_chunk_vertices() const
{
	std::vector<Vectorf> vert;
	std::vector<std::pair<Vectorf, Vectorf>> edges;
	for (const auto& it : collidables)
	{
		if (!it->get_collision()->one_sided)
		{
			const auto mesh = it->get_collision()->mesh;
			vert.push_back(mesh[0]);
			for (int i = 1; i < mesh.size(); i++)
			{
				edges.push_back(std::make_pair(mesh[i - 1], mesh[i]));
				vert.push_back(mesh[i]);
			}
			edges.push_back(std::make_pair(mesh.back(), mesh.front()));
		}
	}
	return std::make_pair(vert, edges);
}