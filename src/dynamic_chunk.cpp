#include "dynamic_chunk.h"

Dynamic_chunk::Dynamic_chunk(std::vector<std::unique_ptr<Chunk>>&& chunks_, 
	std::unordered_map<int, int>&& transition_array_) :
	chunks(std::move(chunks_)), transition_array(transition_array_)
{
	current_chunk = &*chunks.at(transition_array.at(0));
}

void Dynamic_chunk::update_graphics(float dt)
{
	current_chunk->update_graphics(dt);
}

void Dynamic_chunk::update_physics(float dt)
{
	while (message_available())
	{
		Message msg = pop_message();
		if (msg.type != Message::Message_type::MAP_EVENT)
		{
			continue;
		}
		int event_index = std::get<int>(msg.args);
		if (transition_array.contains(event_index))
		{
			current_chunk = &*chunks.at(transition_array.at(event_index));
		}
	}
	current_chunk->update_physics(dt);
}

void Dynamic_chunk::draw_layer(sf::RenderTarget& target, sf::RenderStates states, int layer) const
{
	current_chunk->draw_layer(target, states, layer);
}

void Dynamic_chunk::draw_border(sf::RenderTarget& target, sf::RenderStates states) const
{
	current_chunk->draw_border(target, states);
}

void Dynamic_chunk::draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	current_chunk->draw_moving_collisions(target, states);
}

void Dynamic_chunk::draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	current_chunk->draw_static_collisions(target, states);
}

void Dynamic_chunk::draw_zones(sf::RenderTarget& target, sf::RenderStates states) const
{
	current_chunk->draw_zones(target, states);
}

void Dynamic_chunk::resolve_collisions(Entity& entity) const
{
	current_chunk->resolve_collisions(entity);
}

void Dynamic_chunk::make_zones_interactions(Entity& entity) const
{
	current_chunk->make_zones_interactions(entity);
}

std::pair<std::vector<Vectorf>, std::vector<std::pair<Vectorf, Vectorf>>> Dynamic_chunk::get_chunk_vertices() const
{
	return current_chunk->get_chunk_vertices();
}

sf::FloatRect Dynamic_chunk::get_bounding_rect() const
{
	return current_chunk->get_bounding_rect();
}
