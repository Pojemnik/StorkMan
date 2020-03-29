#pragma once
#include "game.h"
#include <fstream>
#include <iostream>
#include <sstream>

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	Animation* stork_run = NULL;
	Animation* stork_idle = NULL;
	Animation* stork_jump_run = NULL;
	Animation* stork_jump_idle = NULL;
	Animation ship_dockx;
	Animation ship_docky;
	Animation ship_fly;
	Animation ship_idle;
	Animation ship_ext_hdmi;
	Animation ship_undockx;
	Animation ship_undocky;
	Animation ship_ret_hdmi;
	sf::Texture* pieces = NULL;
	std::vector<sf::IntRect> pieces_rect;
	std::map<std::string, const sf::Texture*> textures;
	std::vector<const Dynamic_animation *> animations;
	void load_assets();

private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	Animation* load_animation(std::string path, Vectorf center, sf::FloatRect rect);
	Dynamic_animation* load_dynamic_animation(std::string path);
};
