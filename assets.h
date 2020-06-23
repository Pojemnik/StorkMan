#pragma once
#include "game.h"

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	std::vector<sf::Texture> enemy_textures;
	sf::Texture* pieces = nullptr;
	std::vector<sf::IntRect> pieces_rect;
	std::map<std::string, const sf::Texture*> textures;
	std::vector<const Dynamic_animation *> dynamic_animations;
	std::map<std::string, std::vector<sf::Texture>> animations;
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
	void load_additional_animation(string path, string name, Vectori n,
		Vectori size);

private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_texture(sf::Texture& t, string path, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	void load_shaders();
	void load_animation(std::vector<sf::Texture>& a, sf::Image& img, int x, int y,
		int sx, int sy);//a musi byæ zaalokowane wczeœniej
	Dynamic_animation* load_dynamic_animation(std::string path);
	Animation_tree load_animation_tree(std::string path);
};
