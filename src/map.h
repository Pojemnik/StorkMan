#pragma once
#include <iostream>
#include "entities.h"
#include "level.h"
#include "messaging.h"

class Map : public Message_sender
{
	Vectori size;
	Vectori current_pos;
	std::vector<std::vector<std::unique_ptr<Level>>> levels;
	std::vector<Entity*> entities;
	sf::Sprite background;
	std::vector<Level*> considered_levels;

	void update_levels(float dt, sf::FloatRect screen_rect);
	void resolve_collisions();
	void make_zones_interactions();
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
	void draw_vertices(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_zones(sf::RenderTarget& target, sf::RenderStates states) const;
};