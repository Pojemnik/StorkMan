#pragma once
#include "core.h"
#include <vector>
#include <list>

class Level
{
public:
	Vectori global_pos;
	Vectori global_size;
	bool is_loaded = false;
	//std::list<Object> objects;
	std::vector<Renderable> drawables;
	std::vector<Texturable> texturables;
	//std::list<Transformable> movables;
	//std::list<Animatable> animatables;	//Great name

	Level();
	void addRenderable(Renderable d);
	void addTexturable(Texturable t);
};

class Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<Level> levels;
	std::shared_ptr<Level>** level_placement;
	std::list<std::shared_ptr<std::vector<Renderable>>> drawables;
	std::list<std::shared_ptr<std::vector<Texturable>>> texturables;

	void load_level(Vectori pos);
	void unload_level(Vectori pos);

public:
	Map() = default;
	Map(Vectori dimensions, std::unique_ptr<std::vector<Level>> levels, Vectori start_pos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update();
};