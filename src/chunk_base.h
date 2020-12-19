#pragma once
#include "collisions.h"
#include "interfaces.h"
#include "zones.h"
#include "entities.h"

class Chunk : public Map_object, public Message_receiver
{
public:
	virtual ~Chunk() = 0 {};
	virtual void update_graphics(float dt) = 0;
	virtual void update_physics(float dt, std::vector<int>& msg_up) = 0;
	virtual void draw_layer(sf::RenderTarget& target, sf::RenderStates states, int layer) const = 0;
	virtual void draw_border(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void resolve_collisions(Entity& entity) const = 0;
	virtual void make_zones_interactions(Entity& entity) const = 0;
	virtual std::pair<std::vector<Vectorf>, std::vector<std::pair<Vectorf, Vectorf>>> get_chunk_vertices() const = 0;
	virtual sf::FloatRect get_bounding_rect() const = 0;
};
