#include "level.h"
#include "zones.h"

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
	std::vector<std::shared_ptr<Zone>>&& zones_,
	sf::FloatRect bound_, sf::VertexBuffer&& static_vertices)
	: updatables(std::move(updatables_)), collidables(std::move(collidables_)),
	bound(bound_), zones(std::move(zones_)), static_collision_vertices(std::move(static_vertices))
{
	for (auto& it : drawables_)
	{
		layers[it.first].push_back(it.second);
	}
	border.setPosition(bound.left, bound.top);
	border.setSize({ bound.width, bound.height });
	border.setOutlineColor(sf::Color::Green);
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

const Collidable* Moving_element::get_collidable() const
{
	return &*collidable;
}

sf::FloatRect Moving_element::get_bounding_rect() const
{
	return object->get_bounding_rect();
}

void Moving_element::update(float dt)
{
	updatable->update(dt);
}

void Moving_element::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (is_drawable && on_screen)
	{
		target.draw(*renderable, states);
	}
}

void Moving_element::draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (is_collidable)
	{
		collidable->draw_dynamic_collision(target, states);
	}
}

Moving_element::Moving_element(std::shared_ptr<Updatable> updatable_, int layer_) :
	updatable(std::move(updatable_)), layer(layer_)
{
	renderable = std::dynamic_pointer_cast<Renderable>(updatable);
	is_drawable = (renderable != nullptr);
	collidable = std::dynamic_pointer_cast<Collidable>(updatable);
	is_collidable = (collidable != nullptr);
	object = std::dynamic_pointer_cast<Map_object>(updatable);
}

Level::Level(std::vector<Map_chunk>&& chunks_,
	std::vector<Moving_element>&& moving_, Vectori pos, int code_)
	: chunks(std::move(chunks_)), moving(std::move(moving_)),
	global_pos(pos), code(code_) {}

void Level::update(float dt, sf::FloatRect screen_rect)
{
	std::vector<std::future<void>> futures;
	for (auto& it : chunks)
	{
		if (it.get_bounding_rect().intersects(screen_rect))
		{
			it.on_screen = true;
			futures.push_back(context.thread_pool->push(Level::update_chunk, it, dt));
			//Maybe check if there is something to update in chunk
		}
		else
		{
			it.on_screen = false;
		}
	}
	for (auto& it : moving)
	{
		if (it.get_bounding_rect().intersects(screen_rect))
		{
			it.on_screen = true;
			it.update(dt);
		}
		else
		{
			it.on_screen = false;
		}
	}
	for (auto& it : futures)
	{
		it.get();
	}
}

void Level::update_chunk(int id, Map_chunk& chunk, float dt)
{
	(void)id;
	chunk.update(dt);
}

void Level::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < BOTTOM_LAYERS; i++)
	{
		for (const auto& it : chunks)
		{
			if (it.on_screen)
			{
				it.draw_layer(target, states, i);
			}
		}
		for (const auto& it : moving)
		{
			if (it.layer == i)
			{
				target.draw(it, states);
			}
		}
	}
}

void Level::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = BOTTOM_LAYERS; i < BOTTOM_LAYERS + MIDDLE_LAYERS; i++)
	{
		for (const auto& it : chunks)
		{
			if (it.on_screen)
			{
				it.draw_layer(target, states, i);
			}
		}
		for (const auto& it : moving)
		{
			if (it.layer == i)
			{
				target.draw(it, states);
			}
		}
	}
}

void Level::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = BOTTOM_LAYERS + MIDDLE_LAYERS; i < TOTAL_LAYERS; i++)
	{
		for (const auto& it : chunks)
		{
			if (it.on_screen)
			{
				it.draw_layer(target, states, i);
			}
		}
		for (const auto& it : moving)
		{
			if (it.layer == i)
			{
				target.draw(it, states);
			}
		}
	}
}

void Level::resolve_collisions(std::vector<Entity*>& entities)
{
	for (auto& it : entities)
	{
		for (const auto& chunk_it : chunks)
		{
			if (it->get_collision()->rect.intersects(chunk_it.get_bounding_rect()))
			{
				chunk_it.resolve_collisions(*it);
			}
		}
		for (const auto& moving_it : moving)
		{
			if (moving_it.is_collidable)
			{
				it->resolve_collision(*moving_it.get_collidable());
			}
		}
	}
}

void Level::set_draw_border(bool draw)
{
	draw_border = draw;
}

void Level::set_draw_chunks_borders(bool draw)
{
	draw_chunks_borders = draw;
}

void Level::make_zones_interactions(std::vector<Entity*>& entities)
{
	for (auto& it : entities)
	{
		for (const auto& chunk_it : chunks)
		{
			chunk_it.make_zones_interactions(*it);
		}
	}
}

void Level::draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : chunks)
	{
		if (it.on_screen)
		{
			it.draw_moving_collisions(target, states);
		}
	}
	for (const auto& it : moving)
	{
		it.draw_moving_collisions(target, states);
	}
}

void Level::draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : chunks)
	{
		if (it.on_screen)
		{
			it.draw_static_collisions(target, states);
		}
	}
}

void Level::draw_zones(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : chunks)
	{
		if (it.on_screen)
		{
			it.draw_zones(target, states);
		}
	}
	//Add, when moving damage zones are added
	//for (const auto& it : moving)
	//{
	//	it.draw_zones(target, states);
	//}
}

Vectori Level::get_global_pos() const
{
	return global_pos;
}
