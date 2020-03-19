#pragma once
#include "game.h"

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	Animation* stork_run;
	Animation* stork_idle;
	Animation* stork_jump_run;
	Animation* stork_jump_idle;
	Animation ship_dockx;
	Animation ship_docky;
	Animation ship_fly;
	Animation ship_idle;
	Animation ship_ext_hdmi;
	Animation ship_undockx;
	Animation ship_undocky;
	Animation ship_ret_hdmi;
	std::vector<sf::Texture> pieces;
	std::map<std::string, const sf::Texture*> textures;
	void load_assets();

private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	Animation* load_animation(std::string path, Vectorf center, sf::FloatRect rect);
};