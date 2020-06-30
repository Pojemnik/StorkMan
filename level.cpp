#include "level.h"

Level::Level()
{
}

Level::Level(const Level& level)
	: global_pos(level.global_pos), is_loaded(level.is_loaded),
	global_size(level.global_size), platforms(level.platforms),
	light_sources(level.light_sources), walls(level.walls),
	objects(level.objects), anim_objects(level.anim_objects), 
	pendulums(level.pendulums)
{
	for (auto& p : platforms)
	{
		add_collidable(&p);
		if (p.visible)
		{
			add_to_layer(p);
		}
	}
	for (auto& p : pendulums)
	{
		add_physical(&p);
		add_to_layer(p);
	}
	for (auto& w : walls)
	{
		add_to_layer(w);
	}
	for (auto& o : objects)
	{
		add_to_layer(o);
	}
	for (auto& o : anim_objects)
	{
		add_animatable(&o);
		add_to_layer(o);
	}
}

void Level::add_object(Object o)
{
	objects.push_back(o);
	add_to_layer(o);
}

void Level::add_object(Animated_object o)
{
	anim_objects.push_back(o);
	add_to_layer(o);
}

void Level::add_pendulum(Pendulum p)
{
	pendulums.push_back(p);
	add_to_layer(p);
	add_physical(&p);
}

void Level::add_to_layer(Animatable& a)
{
	if (a.layer < BOTTOM_LAYERS)
		bottom_layers[a.layer].push_back(&a);
	else if (a.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[a.layer - BOTTOM_LAYERS].push_back(&a);
	else if (a.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[a.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&a);
}

void Level::add_to_layer(Texturable& t)
{
	if (t.layer < BOTTOM_LAYERS)
		bottom_layers[t.layer].push_back(&t);
	else if (t.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[t.layer - BOTTOM_LAYERS].push_back(&t);
	else if (t.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[t.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&t);
}

void Level::add_to_layer(Renderable& r)
{
	if (r.layer < BOTTOM_LAYERS)
		bottom_layers[r.layer].push_back(&r);
	else if (r.layer < MIDDLE_LAYERS + BOTTOM_LAYERS)
		middle_layers[r.layer - BOTTOM_LAYERS].push_back(&r);
	else if (r.layer < TOP_LAYERS + MIDDLE_LAYERS + BOTTOM_LAYERS)
		top_layers[r.layer - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(&r);
}

void Level::add_physical(Physical* p)
{
	physicals.push_back(p);
}

void Level::add_collidable(Collidable* c)
{
	collidables.push_back(c);
}

void Level::add_animatable(Animatable* a)
{
	animatables.push_back(a);
}

void Level::add_light_source(Light_source l)
{
	light_sources.push_back(l);
}

void Level::add_platform(Platform p)
{
	platforms.push_back(p);
	add_to_layer(p);
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
	add_to_layer(w);
}