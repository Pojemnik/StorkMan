#pragma once
#include "entities.h"
#include "level.h"

class Map
{
	Vectori size;
	Vectori current_pos;
	std::vector<std::vector<Level>> levels;

public:
	Map(Vectori size_, Vectori pos);
	void add_level(Level&& lvl, Vectori pos);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt, Vectorf player_pos);
};

class old_Map : public sf::Drawable
{
private:
	Vectori size;
	Vectori current_pos;
	std::vector<old_Level> levels;
	old_Level*** level_placement;
	std::list<old_Level*> loaded_levels;
	sf::RenderTexture* light_texture;
	std::vector<Vectorf> map_vertices;
	float global_scale;
	std::array<bool, 8> player_smash;
	std::vector<std::pair<Vectorf, Vectorf>> invisible_map_edges;
	std::vector<std::pair<Vectorf, Vectorf>> map_edges;

	void load_level(Vectori pos);
	void unload_level(std::list<old_Level*>::iterator& lvl);
	void unload_levels_out_of_bounds();
	void load_levels_in_bounds(Vectori pos);
	void place_levels();
	static void update_level(int id, old_Level* lvl, float dt);

public:
	sf::Sprite background;
	sf::Sprite layer2;	//Lepiej rozwi�za�
	Player* player;		//Chyba niezyt eleganckie
	sf::Vector2f level_size = 
		{ 100 * context.global_scale,100 * context.global_scale };
	Light light;
	sf::Sprite light_sprite;

	old_Map();
	old_Map(Vectori dimensions, std::vector<old_Level>& lvls, Vectori start_pos,
		sf::Texture& bg_tex, sf::Texture& layer2_tex, sf::Texture* light_tex);
	void calc_map_vertices();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_backgrounds(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_map_vertices(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_damage_zones(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void pre_draw();
	void recalc_light();
	void rescale(float new_global_scale);
};