#include "dynamic_chunk.h"

Dynamic_chunk::Dynamic_chunk(std::vector<std::unique_ptr<Chunk>>&& chunks_, 
	std::unordered_map<int, int>&& transition_array_,
	std::unordered_map<int, std::vector<std::pair<float, int>>>&& time_events_) :
	chunks(std::move(chunks_)), transition_array(transition_array_),
	time_events(time_events_), Message_sender(Message_sender_type::CHUNK)
{
	current_chunk = &*chunks.at(transition_array.at(0));
	for (auto& it : chunks)
	{
		add_receiver(&*it);
	}
}

void Dynamic_chunk::update_graphics(float dt)
{
	current_chunk->update_graphics(dt);
}

void Dynamic_chunk::update_physics(float dt, std::vector<int>& msg_up)
{
	float last_time = time;
	time += dt;
	while (message_available())
	{
		Message msg = pop_message();
		if (msg.type == Message::Message_type::MAP_EVENT)
		{
			int event_index = std::get<int>(msg.args);
			if (transition_array.contains(event_index))
			{
				current_chunk = &*chunks.at(transition_array.at(event_index));
				current_chunk_index = transition_array.at(event_index);
				time = 0;
			}
		}
		if (msg.type == Message::Message_type::MOUSE_CLICKED)
		{
			send_message(Message::Message_type::MOUSE_CLICKED, msg.args);
		}
	}
	if (time_events.contains(current_chunk_index))
	{
		for (const auto& it : time_events.at(current_chunk_index))
		{
			if (time >= it.first && last_time < it.first)
			{
				msg_up.push_back(it.second);
			}
		}
	}
	current_chunk->update_physics(dt, msg_up);
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
