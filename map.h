#pragma once
#include "level.h"
#include "light.h"

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
	float global_scale;

	void load_level(Vectori pos);
	void unload_level(std::list<Level*>::iterator& lvl);

public:
	std::vector<std::pair<Vectorf, Vectorf>> map_edges;
	sf::Sprite background;
	sf::Sprite layer2;	//Lepiej rozwi¹zaæ
	Player* player;		//Chyba niezyt eleganckie
	sf::Vector2f level_size = { 100 * context.global_scale,100 * context.global_scale };
	Light light;

	Map();
	Map(Vectori dimensions, std::vector<Level>& levels, Vectori start_pos, sf::Texture& bg, sf::Texture* light_tex);
	void calc_map_vertices();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void redraw();
	void rescale(float new_global_scale);
};