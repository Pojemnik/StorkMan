#include "level.h"

Level::Level()
{
}

Level::Level(const Level& level)
	: global_pos(level.global_pos), is_loaded(level.is_loaded),
	global_size(level.global_size), platforms(level.platforms),
	light_sources(level.light_sources), walls(level.walls), objects(level.objects)
{
	for (auto& it : platforms)
	{
		add_colidable(&it);
	}
}

void Level::add_object(Object o)
{
	objects.push_back(o);
}

void Level::add_physical(Physical* p)
{
	physicals.push_back(p);
}

void Level::add_colidable(Colidable* c)
{
	colidables.push_back(c);
}

void Level::add_light_source(Light_source l)
{
	light_sources.push_back(l);
}

void Level::add_platform(Platform p)
{
	platforms.push_back(p);
	add_colidable(&p);
}

void Level::rescale(float ratio)
{
	for (auto& it : platforms)
		it.rescale(ratio);
	for (auto& it : walls)
		it.rescale(ratio);
}

void Level::add_wall(Wall w)
{
	walls.push_back(w);
}