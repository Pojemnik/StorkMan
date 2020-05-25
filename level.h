#pragma once
#include "game.h"
#include "light.h"

struct Level
{
	Vectori global_pos;
	Vectori global_size;
	bool is_loaded = false;
	std::vector<Physical*> physicals;
	std::vector<Collidable*> collidables;
	std::vector<Platform> platforms;
	std::vector<Light_source> light_sources;
	std::vector<Wall> walls;
	std::vector<Object> objects;
	std::array<std::vector<sf::Drawable*>, BOTTOM_LAYERS> bottom_layers;
	std::array<std::vector<sf::Drawable*>, MIDDLE_LAYERS> middle_layers;
	std::array<std::vector<sf::Drawable*>, TOP_LAYERS> top_layers;

	Level();
	Level(const Level& level);
	void add_physical(Physical* p);
	void add_collidable(Collidable* c);
	void add_platform(Platform p);
	void add_light_source(Light_source l);
	void add_wall(Wall w);
	void add_object(Object o);
	void rescale(float ratio);
};
