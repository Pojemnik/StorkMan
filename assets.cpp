#include "assets.h"
#include<sstream>

void Assets::load_texture(sf::Texture& t, sf::Image& img, int x, int y, int sx, int sy, bool rep)
{
	if (!t.loadFromImage(img, sf::IntRect(x, y, sx, sy)))
	{
		std::cout << "Texture loading error" << std::endl;
		return;
	}
	else
	{
		if (rep)
		{
			t.setRepeated(true);
		}
	}
}

Animation* Assets::load_animation(std::string path, Vectorf center, sf::FloatRect rect)
{
	std::vector<sf::Texture> v;
	load_textures(v, path, false);
	Animation* a = new Animation(v, center, rect);
	return a;
}

void Assets::load_textures(std::vector<sf::Texture>& v, std::string path, bool rep)
{
	int a, b, c, d;
	int l = path.rfind("_ss_");
	if (l == std::string::npos)
	{
		l = -1;
	}
	l++;
	int r = path.find(".", l);
	std::string tmp = path.substr(l, r - l);
	for (auto& it : tmp)
	{
		if (it > '9' || it < '0')
			it = ' ';

	}
	std::stringstream tmps(tmp);
	if (!(tmps >> a >> b >> c >> d))
	{
		std::cout << "error reading sizes " + path << std::endl;
		return;
	}
	v.reserve(c * d);
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		std::cout << "Error while loading" + path << std::endl;
		return;
	}
	for (int j = 0; j < d; j++)
	{
		for (int i = 0; i < c; i++)
		{
			v.push_back(sf::Texture());
			load_texture(v.back(), image, i * a, j * b, a, b, rep);
		}
	}
}

void Assets::load_assets()
{
	std::cout << "Loading assets..." << std::endl;
	sf::FloatRect tmp(210, 100, 105, 340);
	stork_run = load_animation("img/new_a/run_ss_512_512_is_8_10.png", { 0, 0 },tmp);
	stork_idle = load_animation("img/new_a/idle_ss_512_512_is_10_12.png", { 0, 0 },tmp);
	stork_jump_run = load_animation("img/new_a/skok w biegu_ss_512_512_is_10_12.png", { 0, 0 },tmp);
	stork_jump_idle = load_animation("img/new_a/skok z postoju_ss_512_512_is_10_14.png", { 0, 0 },tmp);
	load_textures(map_textures, "img/tex_ss_64_64_is_1_17.png", true);
	//load_textures(ship_dockx, "img/ships/DokowanieX_ss_436_87_is_10_12.png", false);
	//load_textures(ship_docky, "img/ships/DokowanieY_ss_443_442_is_15_20.png", false);
	//load_textures(ship_fly, "img/ships/Lot_ss_466_87_is_6_10.png", false);
	//load_textures(ship_idle, "img/ships/Postój_ss_446_87_is_6_10.png", false);
	//load_textures(ship_ext_hdmi, "img/ships/Wysuniêcie HDMI_ss_446_209_is_6_10.png", false);
	//load_textures(ship_ret_hdmi, "img/ships/Schowanie HDMI_ss_446_209_is_6_10.png", false);
	//load_textures(ship_undockx, "img/ships/WydokowanieX_ss_436_87_is_6_10.png", false);
	//load_textures(ship_undocky, "img/ships/WydokowanieY_ss_443_442_is_10_12.png", false);

	textures["bricks,0"] = &map_textures[3];
	textures["bricks,1"] = &map_textures[4];
	textures["bricks,2"] = &map_textures[5];
	textures["bricks,3"] = &map_textures[6];
	std::cout << "done!" << std::endl;
}