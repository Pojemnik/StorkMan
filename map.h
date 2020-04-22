#pragma once
#include "level.h"

const sf::Vector2f level_size = { 100 * global_scale,100 * global_scale };

class Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<Level> levels;
	Level*** level_placement;
	std::list<Level*> loaded_levels;
	sf::Sprite map_sprite;
	sf::RenderTexture* map_texture;
	std::vector<Vectorf> map_vertices;

	void load_level(Vectori pos);
	void unload_level(Vectori pos);
	void unload_level(std::list<Level*>::iterator& lvl);
	std::pair<float, Vectorf> cast_ray(Vectorf source, Vectorf alfa) const;
	std::vector<std::pair<float, Vectorf>> calc_light_source(Vectorf source);
	void redraw();

public:
	std::vector<std::pair<Vectorf, Vectorf>> map_edges;
	void calc_map_vertices();
	sf::Sprite background;
	sf::Sprite lightmap;
	sf::Sprite layer2;	//Lepiej rozwi¹zaæ
	Player* player;		//Chyba niezyt eleganckie

	Map();
	Map(Vectori dimensions, std::vector<Level>& levels, Vectori start_pos, sf::Texture& bg);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	sf::Texture calc_light(std::vector<Vectorf>& sources, sf::Transform transform);
};