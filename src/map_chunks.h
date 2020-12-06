#pragma once
#include "compound_map_objects.h"
#include "map_sounds.h"
#include "zones.h"

class Map_chunk : public Map_object
{
	std::vector<std::shared_ptr<Physical_updatable>> p_updatables;
	std::vector<std::shared_ptr<Graphical_updatable>> g_updatables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	std::vector<std::shared_ptr<Zone>> zones;
	std::array<std::vector<std::shared_ptr<Renderable>>, TOTAL_LAYERS> layers;
	sf::FloatRect bound;
	sf::RectangleShape border;
	sf::VertexBuffer static_collision_vertices;

public:
	bool on_screen = false;
	Map_chunk(std::vector<std::shared_ptr<Physical_updatable>>&& p_updatables_,
		std::vector<std::shared_ptr<Graphical_updatable>>&& g_updatables_,
		std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
		std::vector<std::shared_ptr<const Collidable>>&& collidables_,
		std::vector<std::shared_ptr<Zone>>&& zones_,
		sf::FloatRect bound_, sf::VertexBuffer&& static_vertices);
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