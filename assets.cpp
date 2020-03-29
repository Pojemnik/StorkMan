#include "assets.h"

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

Dynamic_animation* Assets::load_dynamic_animation(std::string path)
{
	std::ifstream f(path);
	int n;
	f >> n;
	std::vector<std::array<float, 21>> kf(n);
	std::vector<int> l(n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			f >> kf[i][j];
		}
	}
	for (int i = 0; i < n; i++)
		f >> l[i];
	return new Dynamic_animation(kf, l);
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
	pieces = new sf::Texture();
	pieces->loadFromFile("img/stork/parts_ss_128_128_is_3_9.png");
	for (int i = 0; i < 27; i++)
		pieces_rect.push_back({128*(i%3),128*(i/3),128,128});
	animations.push_back(load_dynamic_animation("animations/stork/idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run2.txt"));
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