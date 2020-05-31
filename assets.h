#pragma once
#include "game.h"

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	sf::Texture* pieces = nullptr;
	std::vector<sf::IntRect> pieces_rect;
	std::map<std::string, const sf::Texture*> textures;
	std::vector<const Dynamic_animation *> animations;
	sf::Texture* bg = nullptr;
	sf::Texture* layer2 = nullptr;
	sf::Texture* light = nullptr;
	sf::Texture* console_bg = nullptr;
	Animation_tree stork_tree;
	sf::Shader blurv, blurh, white;
	sf::Font storkfont, consola;
	sf::Image icon;
	
	void load_assets();
	void load_additional_texture(std::string path, std::string name, int repeat);

private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	void load_shaders();
	Dynamic_animation* load_dynamic_animation(std::string path);
	Animation_tree load_animation_tree(std::string path);
};
