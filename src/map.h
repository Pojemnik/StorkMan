#pragma once
#include <iostream>
#include "entities.h"
#include "level.h"

class Map
{
	Vectori size;
	Vectori current_pos;
	std::vector<std::vector<Level>> levels;
	std::vector<Entity*> entities;
	sf::Sprite background;

	void update_levels(float dt, sf::FloatRect screen_rect);
	void resolve_collisions();
	void call_on_considered_levels(std::function<void(Level&)> foo);

public:
	Map(Vectori size_, Vectori pos, const sf::Texture* bg_tex);
	void add_level(Level&& lvl);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt, Vectorf player_pos, sf::FloatRect screen_rect);
	void add_entity(Entity* entity);
	void set_draw_chunks_borders(bool draw);
};