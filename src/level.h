#pragma once
#include <unordered_set>
#include "compound_map_objects.h"
#include "map_sounds.h"
#include "map_chunk.h"
#include "dynamic_chunk.h"
#include "messaging.h"

class Zone;
class Entity;

class Level : public Message_sender, public Message_receiver
{
	std::vector<std::unique_ptr<Chunk>> chunks;
	std::vector<bool> chunks_on_screen;
	Vectori global_pos;
	std::vector<Map_sound> sounds;
	std::vector<sf::CircleShape> sound_sources;
	sf::VertexBuffer sound_borders;
	bool draw_border = false;//Currently unused
	bool draw_chunks_borders = false;
	bool draw_sound_sources = false;

public:
	const int code;

	Level() = default;
	Level(std::vector<std::unique_ptr<Chunk>>&& chunks_, std::vector<Map_sound>&& sounds_, Vectori pos, int code_);
	void update_physics(float dt, sf::FloatRect screen_rect, std::vector<int>& msg_up);
	void update_graphics(float dt, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(std::vector<Entity*>& entities);
	void set_draw_border(bool draw);//Currently unused
	void set_draw_chunks_borders(bool draw);
	void set_draw_sound_sources(bool draw);
	void make_zones_interactions(std::vector<Entity*>& entities);
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unordered_set<const Map_sound*, std::hash<const Map_sound*>, Map_sound_compare>
		get_current_map_sounds(Vectorf player_pos) const;
	Vectori get_global_pos() const;
};