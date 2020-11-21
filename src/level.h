#pragma once
#include <unordered_set>
#include "compound_map_objects.h"
#include "map_sounds.h"
#include "map_chunks.h"

class Zone;
class Entity;

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

class Level
{
	std::vector<Map_chunk> chunks;
	Vectori global_pos;
	std::vector<Moving_element> moving;
	std::vector<Map_sound> sounds;
	bool draw_border = false;//Currently unused
	bool draw_chunks_borders = false;

	static void update_chunk(int id, Map_chunk& chunk, float dt);

public:
	const int code;

	Level() = default;
	Level(std::vector<Map_chunk>&& chunks_,
		std::vector<Moving_element>&& moving_, std::vector<Map_sound>&& sounds_,
		Vectori pos, int code_);
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
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unordered_set<const Map_sound*, std::hash<Map_sound*>, Map_sound_compare>
		get_current_map_sounds(Vectorf player_pos) const;
	Vectori get_global_pos() const;
};