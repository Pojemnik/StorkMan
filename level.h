#pragma once
#include "game.h"
#include "light.h"

struct Level
{
	Vectori global_pos;
	Vectori global_size;
	bool is_loaded = false;
	std::vector<Renderable*> drawables;
	std::vector<Texturable*> texturables;
	std::vector<Physical*> physicals;
	std::vector<Colidable*> colidables;
	std::vector<Platform> platforms;
	std::vector<Light_source> light_sources;
	std::vector<Wall> walls;

	Level();
	Level(const Level& level);
	void add_renderable(Renderable* d);
	void add_texturable(Texturable* t);
	void add_physical(Physical* p);
	void add_colidable(Colidable* c);
	void add_platfrom(Platform p);
	void add_light_source(Light_source l);
	void add_wall(Wall w);
	void rescale(float ratio);
};
