#pragma once
#include <iostream>
#include <unordered_set>
#include "entities.h"
#include "level.h"
#include "messaging.h"
#include "map_sound_info.h"

class Map : public Message_sender
{
	Vectori size;
	Vectori current_pos;
	std::vector<std::vector<std::unique_ptr<Level>>> levels;
	std::vector<Entity*> entities;
	sf::Sprite background;
	std::vector<Level*> considered_levels;
	std::unordered_set<const Map_sound*, std::hash<const Map_sound*>, Map_sound_compare> last_map_sounds;

	void get_considered_levels();

public:
	Map(Vectori size_, Vectori pos, const sf::Texture* bg_tex);
	void init();
	void add_level(std::unique_ptr<Level>&& lvl);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt, Vectorf player_pos, sf::FloatRect screen_rect);
	void add_entity(Entity* entity);
	void set_draw_chunks_borders(bool draw);
	void set_draw_sound_sources(bool draw);
	void draw_vertices(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
};