#pragma once
#include <unordered_map>
#include "compound_map_objects.h"
#include "map_sounds.h"
#include "map_chunk.h"
#include "dynamic_chunk.h"
#include "messaging.h"
#include "timed_events.h"

class Zone;
class Entity;

class Level : public Message_sender, public Message_receiver
{
	std::vector<std::unique_ptr<Chunk>> chunks;
	std::vector<bool> chunks_on_screen;
	Vectori global_pos;
	std::vector<Map_sound> sounds;
	std::vector<Timed_event> timed_events;
	std::vector<sf::CircleShape> sound_sources;
	std::vector<int> received_events;
	sf::VertexBuffer sound_borders;
	bool draw_chunks_borders = false;
	bool draw_sound_sources = false;

public:
	const int code;

	Level() = default;
	Level(std::vector<std::unique_ptr<Chunk>>&& chunks_,
		std::vector<Map_sound>&& sounds_, std::vector<Timed_event> timed_events_,
		Vectori pos, int code_);
	void update_physics(float dt, sf::FloatRect screen_rect, std::vector<int>& msg_up);
	void update_graphics(float dt, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void resolve_collisions(std::vector<Entity*>& entities);
	void set_draw_chunks_borders(bool draw);
	void set_draw_sound_sources(bool draw);
	void make_zones_interactions(std::vector<Entity*>& entities);
	void draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unordered_map<const Map_sound*, int, std::hash<const Map_sound*>, Map_sound_compare>
		get_current_map_sounds(std::vector<Vectorf> player_pos);
	Vectori get_global_pos() const;
};