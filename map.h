#pragma once
#include "core.h"
#include <vector>
#include <list>

class Level
{
public:
	Vectori global_pos;
	bool is_loaded = false;
	//std::list<Object> objects;
	std::vector<Drawable> drawables;
	std::vector<Texturable> texturables;
	//std::list<Transformable> movables;
	//std::list<Animatable> animatables;	//Great name

	Level();
	void load();
	void unload();
	void addDrawable(Drawable d);
	void addTexturable(Texturable t);
};

class Map
{
private:
	Vectori global_pos;
	std::list<Level> levels;
	std::list<std::shared_ptr<Level>> loaded_levels;
	std::shared_ptr<Level>** level_placement;

public:
	Map(Vectori dimensions, std::unique_ptr<std::list<Level>> levels);
	void update();
	void draw();
};