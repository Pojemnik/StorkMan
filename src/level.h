#pragma once
#include "compound_map_objects.h"
#include "entities.h"

class Moving_element : public Updatable, public Renderable, public Map_object
{
	std::shared_ptr<Updatable> updatable;
	std::shared_ptr<Renderable> renderable;
	std::shared_ptr<Map_object> object;
	std::shared_ptr<Collidable> collidable;
	bool is_drawable;

public:
	int layer;
	bool on_screen;
	bool is_collidable;

	const Collidable* get_collidable() const;
	sf::FloatRect get_bounding_rect() const;
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	Moving_element(std::shared_ptr<Updatable> updatable_, int layer_ = 0);
};

class Map_chunk : public Updatable, public Map_object
{
	std::vector<std::shared_ptr<Updatable>> updatables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	std::vector<std::shared_ptr<Zone>> zones;
	std::array<std::vector<std::shared_ptr<Renderable>>, BOTTOM_LAYERS> bottom_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, MIDDLE_LAYERS> middle_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, TOP_LAYERS> top_layers;
	sf::FloatRect bound;
	sf::RectangleShape border;
	sf::VertexBuffer static_collision_vertices;

public:
	bool on_screen = false;
	Map_chunk(std::vector<std::shared_ptr<Updatable>>&& updatables_,
		std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
		std::vector<std::shared_ptr<const Collidable>>&& collidables_,
		std::vector<std::shared_ptr<Zone>>&& zones_,
		sf::FloatRect bound_, sf::VertexBuffer&& static_vertices);
	void update(float dt);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_border(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(Entity& entity) const;
	void make_zones_interactions(Entity& entity) const;
	sf::FloatRect get_bounding_rect() const;
};

class Level
{
	std::vector<Map_chunk> chunks;
	Vectori global_pos;
	std::vector<Moving_element> moving;
	bool draw_border = false;//Currently unused
	bool draw_chunks_borders = false;

	static void update_chunk(int id, Map_chunk& chunk, float dt);

public:
	Level() = default;
	Level(std::vector<Map_chunk>&& chunks_,
		std::vector<Moving_element>&& moving_, Vectori pos);
	void update(float dt, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(std::vector<Entity*>& entities);
	void set_draw_border(bool draw);//Currently unused
	void set_draw_chunks_borders(bool draw);
	void make_zones_interactions(std::vector<Entity*>& entities);
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	Vectori get_global_pos() const;
};