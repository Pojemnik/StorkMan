#include "level.h"

Level::Level()
{
}

Level::Level(const Level& level)
	: global_pos(level.global_pos), is_loaded(level.is_loaded),
	global_size(level.global_size), platforms(level.platforms),
	light_sources(level.light_sources), walls(level.walls),
	objects(level.objects)
{
	for (auto& w : platforms)
	{
		add_collidable(&w);
	}
	for (auto& w : platforms)
	{
		if (w.visible)
		{
			if (w.layer < BOTTOM_LAYERS)
				bottom_layers[w.layer].push_back(&w);
			else if (w.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
				middle_layers[w.layer - BOTTOM_LAYERS].push_back(&w);
			else if (w.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
				top_layers[w.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&w);
		}
	}
	for (auto& w : walls)
	{
		if (w.layer < BOTTOM_LAYERS)
			bottom_layers[w.layer].push_back(&w);
		else if (w.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
			middle_layers[w.layer - BOTTOM_LAYERS].push_back(&w);
		else if (w.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
			top_layers[w.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&w);
	}
	for (auto& w : objects)
	{
		if (w.layer < BOTTOM_LAYERS)
			bottom_layers[w.layer].push_back(&w);
		else if (w.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
			middle_layers[w.layer - BOTTOM_LAYERS].push_back(&w);
		else if (w.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
			top_layers[w.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&w);
	}
}

void Level::add_object(Object o)
{
	objects.push_back(o);
	if (o.layer < BOTTOM_LAYERS)
		bottom_layers[o.layer].push_back(&o);
	else if (o.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[o.layer - BOTTOM_LAYERS].push_back(&o);
	else if (o.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[o.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&o);
}

void Level::add_physical(Physical* p)
{
	physicals.push_back(p);
}

void Level::add_collidable(Collidable* c)
{
	collidables.push_back(c);
}

void Level::add_light_source(Light_source l)
{
	light_sources.push_back(l);
}

void Level::add_platform(Platform p)
{
	platforms.push_back(p);
	if (p.layer < BOTTOM_LAYERS)
		bottom_layers[p.layer].push_back(&p);
	else if (p.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[p.layer - BOTTOM_LAYERS].push_back(&p);
	else if (p.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[p.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&p);
	add_collidable(&p);
}

void Level::rescale(float ratio)
{
	for (auto& w : platforms)
		w.rescale(ratio);
	for (auto& w : walls)
		w.rescale(ratio);
	for (auto& w : objects)
		w.rescale(ratio);
}

void Level::add_wall(Wall w)
{
	walls.push_back(w);
	if (w.layer < BOTTOM_LAYERS)
		bottom_layers[w.layer].push_back(&w);
	else if (w.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[w.layer - BOTTOM_LAYERS].push_back(&w);
	else if (w.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[w.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&w);
}