#pragma once
#include "game.h"
#include <vector>
#include <list>

const sf::Vector2f level_size = { 100 * global_scale,100 * global_scale };

class Level
{
public:
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
};

class Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<Level> levels;
	Level*** level_placement;
	std::list<Level*> loaded_levels;
	std::vector<std::pair<Vectorf, Vectorf>> map_vertices;
	void load_level(Vectori pos);
	void unload_level(Vectori pos);
	void unload_level(std::list<Level*>::iterator& lvl);
	void calc_map_vertices();
	std::pair<float, Vectorf> cast_ray(Vectorf source, Vectorf alfa) const;
	std::vector<std::pair<float, Vectorf>> calc_light_source(Vectorf source) const;
public:
	sf::Sprite background;
	sf::Sprite lightmap;
	sf::Sprite layer2;	//Lepiej rozwi¹zaæ
	Player* player;		//Chyba niezyt eleganckie

	Map() = default;
	Map(Vectori dimensions, std::vector<Level>& levels, Vectori start_pos, sf::Texture& bg);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void generate_lightmap(sf::RenderStates states);
};