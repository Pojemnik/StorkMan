#pragma once
#include "game.h"
#include <vector>
#include <list>

class Level
{
public:
	Vectori global_pos;
	Vectori global_size;
	bool is_loaded = false;
	//std::list<Object> objects;
	std::vector<Renderable*> drawables;
	std::vector<Texturable*> texturables;
	std::vector<Physical*> physicals;
	std::vector<Colidable*> colidables;
	std::vector<Platform> platforms;
	//std::list<Transformable> movables;
	//std::list<Animatable> animatables;	//Great name

	Level();
	Level(const Level &level);
	void addRenderable(Renderable* d);
	void addTexturable(Texturable* t);
	void addPhysical(Physical* p);
	void addColidable(Colidable* c);
	void addPlatfrom(Platform p);
};

class Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<Level> levels;
	Level*** level_placement;
	std::list<Level*> loaded_levels;

	void load_level(Vectori pos);
	void unload_level(Vectori pos);

public:
	Player* player;//Chyba niezyt eleganckie

	Map() = default;
	Map(Vectori dimensions, std::vector<Level> &levels, Vectori start_pos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update();
};