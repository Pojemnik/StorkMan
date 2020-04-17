#pragma once
#include "game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	sf::Texture* pieces = NULL;
	std::vector<sf::IntRect> pieces_rect;
	std::map<std::string, const sf::Texture*> textures;
	std::vector<const Dynamic_animation *> animations;
	sf::Texture* bg;
	sf::Texture* layer2;
	sf::Texture* light;
	Animation_tree stork_tree;
	void load_assets();

private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	Animation* load_animation(std::string path, Vectorf center, sf::FloatRect rect);
	Dynamic_animation* load_dynamic_animation(std::string path);
	Animation_tree load_animation_tree(std::string path);
};
