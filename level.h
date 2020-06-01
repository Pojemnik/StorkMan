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
	std::vector<Animatable*> animatables;
	std::vector<Platform> platforms;
	std::vector<Light_source> light_sources;
	std::vector<Wall> walls;
	std::vector<Object> objects;
	std::vector<Animated_object> anim_objects;
	std::array<std::vector<sf::Drawable*>, BOTTOM_LAYERS> bottom_layers;
	std::array<std::vector<sf::Drawable*>, MIDDLE_LAYERS> middle_layers;
	std::array<std::vector<sf::Drawable*>, TOP_LAYERS> top_layers;

	Level();
	Level(const Level& level);
	void add_physical(Physical* p);
	void add_collidable(Collidable* c);
	void add_animatable(Animatable* a);
	void add_platform(Platform p);
	void add_light_source(Light_source l);
	void add_wall(Wall w);
	void add_object(Object o);
	void add_object(Animated_object o);
	void rescale(float ratio);
	void add_to_layer(Texturable& t);
	void add_to_layer(Renderable& r);
	void add_to_layer(Animatable& a);
};
