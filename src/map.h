#pragma once
#include "entities.h"
#include "level.h"

class Map
{
	Vectori size;
	Vectori current_pos;
	std::vector<std::vector<Level>> levels;
	std::vector<Physical> physicals;

	void update_levels(float dt, sf::FloatRect screen_rect);
	void resolve_collisions();
	void call_on_considered_levels(std::function<void(Level&)> foo);
	void call_on_considered_levels(std::function<void(const Level&)> foo) const;

public:
	Map(Vectori size_, Vectori pos);
	void add_level(Level&& lvl);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt, Vectorf player_pos, sf::FloatRect screen_rect);
};