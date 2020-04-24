#pragma once
#include "game.h"

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

	Level();
	Level(const Level& level);
	void addRenderable(Renderable* d);
	void addTexturable(Texturable* t);
	void addPhysical(Physical* p);
	void addColidable(Colidable* c);
	void addPlatfrom(Platform p);
	void rescale(float ratio);
};
