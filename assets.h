#pragma once
#include "core.h"

class Assets
{
public:
	std::vector<sf::Texture> map_textures;
	std::vector<sf::Texture> stork_run;
	std::vector<sf::Texture> stork_idle;
	std::vector<sf::Texture> ship_dockx;
	std::vector<sf::Texture> ship_docky;
	std::vector<sf::Texture> ship_fly;
	std::vector<sf::Texture> ship_idle;
	std::vector<sf::Texture> ship_ext_hdmi;
	std::vector<sf::Texture> ship_undockx;
	std::vector<sf::Texture> ship_undocky;
	std::vector<sf::Texture> ship_ret_hdmi;
	std::map<std::string, std::shared_ptr<sf::Texture>> textures;
	void load_assets();
private:
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
};