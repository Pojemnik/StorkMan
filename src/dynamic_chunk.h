#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include "chunk_base.h"
#include "messaging.h"

class Dynamic_chunk : public Chunk, public Message_receiver
{
	std::vector<std::unique_ptr<Chunk>> chunks;
	Chunk* current_chunk;
	std::unordered_map<int, int> transition_array;

public:
	Dynamic_chunk(std::vector<std::unique_ptr<Chunk>>&& chunks_, std::unordered_map<int, int>&& transition_array_);
	void update_graphics(float dt);
	void update_physics(float dt);
	void draw_layer(sf::RenderTarget& target, sf::RenderStates states, int layer) const;
	void draw_border(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(Entity& entity) const;
	void make_zones_interactions(Entity& entity) const;
	std::pair<std::vector<Vectorf>, std::vector<std::pair<Vectorf, Vectorf>>> get_chunk_vertices() const;
	sf::FloatRect get_bounding_rect() const;
};