#pragma once
#include "compound_map_objects.h"

class Zone;
class Entity;


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
	sf::FloatRect get_bounding_rect() const;
};

class Level
{
	std::vector<Map_chunk> chunks;
	Vectori global_pos;
	bool draw_border = false;//Currently unused
	bool draw_chunks_borders = false;

	static void update_chunk_graphics(int id, Map_chunk& chunk, float dt);
	static void update_chunk_physics(int id, Map_chunk& chunk, float dt);


public:
	const int code;

	Level() = default;
	Level(std::vector<Map_chunk>&& chunks_, Vectori pos, int code_);
	void update_physics(float dt, sf::FloatRect screen_rect);
	void update_graphics(float dt, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(std::vector<Entity*>& entities);
	void set_draw_border(bool draw);//Currently unused
	void set_draw_chunks_borders(bool draw);
	void make_zones_interactions(std::vector<Entity*>& entities);
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	Vectori get_global_pos() const;
};