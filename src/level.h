#pragma once
#include "worldparts.h"
#include "platforms.h"
#include "physics.h"

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
	Moving_element(std::shared_ptr<Updatable> updatable_, int layer_);
};

class Map_chunk : public Updatable, public Map_object
{
	std::vector<std::shared_ptr<Updatable>> updatables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	std::array<std::vector<std::shared_ptr<Renderable>>, BOTTOM_LAYERS> bottom_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, MIDDLE_LAYERS> middle_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, TOP_LAYERS> top_layers;
	sf::FloatRect bound;

public:
	bool on_screen;
	Map_chunk(std::vector<std::shared_ptr<Updatable>>&& updatables_,
		std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
		std::vector<std::shared_ptr<const Collidable>>&& collidables_,
		sf::FloatRect bound_);
	void update(float dt);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(Physical& physical) const;
	sf::FloatRect get_bounding_rect() const;
};

class Level
{
	std::vector<Map_chunk> chunks;
	Vectori global_pos;
	std::vector<Moving_element> moving;

public:
	Level(std::vector<Map_chunk>&& chunks_,
		std::vector<Moving_element>&& moving_, Vectori pos);
	void update(float dt, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(std::vector<Physical>& entities);
};