#pragma once
#include "level.h"

class Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<Level> levels;
	Level*** level_placement;
	std::list<Level*> loaded_levels;
	sf::RenderTexture* light_texture;
	std::vector<Vectorf> map_vertices;
	float global_scale;
	std::array<bool, 8> player_smash;

	void load_level(Vectori pos);
	void unload_level(std::list<Level*>::iterator& lvl);
	void unload_levels_out_of_bounds();
	void load_levels_in_bounds(Vectori pos);
	void place_levels();

public:
	std::vector<std::pair<Vectorf, Vectorf>> map_edges;
	sf::Sprite background;
	sf::Sprite layer2;	//Lepiej rozwi¹zaæ
	Player* player;		//Chyba niezyt eleganckie
	sf::Vector2f level_size = 
		{ 100 * context.global_scale,100 * context.global_scale };
	Light light;
	sf::Sprite light_sprite;

	Map();
	Map(Vectori dimensions, std::vector<Level>& lvls, Vectori start_pos,
		sf::Texture& bg_tex, sf::Texture& layer2_tex, sf::Texture* light_tex);
	void calc_map_vertices();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_backgrounds(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void pre_draw();
	void recalc_light();
	void rescale(float new_global_scale);
};