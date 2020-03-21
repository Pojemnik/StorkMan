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
	for (int i = 0; i < 27; i++)
		pieces.push_back(new sf::Texture);
	pieces[BELLY]->loadFromFile("img/stork/parts/BRZUCH_ST_00.PNG");
	pieces[L_HAND]->loadFromFile("img/stork/parts/DLON_L_ST_00.PNG");
	pieces[R_HAND]->loadFromFile("img/stork/parts/DLON_P_ST_00.PNG");
	pieces[HEAD]->loadFromFile("img/stork/parts/GLOWA_ST_00.png");
	pieces[CHEST]->loadFromFile("img/stork/parts/KL_PIERS_ST_00.PNG");
	pieces[L_CALF]->loadFromFile("img/stork/parts/LYDKA_ST_00.PNG");
	pieces[R_CALF] = pieces[L_CALF];
	pieces[PELVIS]->loadFromFile("img/stork/parts/MIEDNICA_ST_00.PNG");
	pieces[L_FOREARM]->loadFromFile("img/stork/parts/PRZEDRAMIE_ST_00.PNG");
	pieces[R_FOREARM] = pieces[L_FOREARM];
	pieces[L_ARM]->loadFromFile("img/stork/parts/RAMIE_ST_00.PNG");
	pieces[R_ARM] = pieces[L_ARM];
	pieces[L_FOOT]->loadFromFile("img/stork/parts/STOPA_ST_00.PNG");
	pieces[R_FOOT] = pieces[L_FOOT];
	pieces[L_TIGH]->loadFromFile("img/stork/parts/UDO_ST_00.PNG");
	pieces[R_TIGH] = pieces[L_TIGH];
	pieces[R_WING_1]->loadFromFile("img/stork/parts/SKRZYDLO_ST_00.png");
	pieces[R_WING_2] = pieces[R_WING_1];
	pieces[R_WING_3] = pieces[R_WING_1];
	pieces[R_WING_4] = pieces[R_WING_1];
	pieces[R_WING_5] = pieces[R_WING_1];
	pieces[L_WING_1] = pieces[R_WING_1];
	pieces[L_WING_2] = pieces[R_WING_1];
	pieces[L_WING_3] = pieces[R_WING_1];
	pieces[L_WING_4] = pieces[R_WING_1];
	pieces[L_WING_5] = pieces[R_WING_1];
	pieces[TAIL]->loadFromFile("img/stork/parts/OGON_ST_00.png");
	animations.push_back(load_dynamic_animation("animations/stork/idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run.txt"));
	stork_run = load_animation("img/new_a/run_ss_512_512_is_8_10.png", { 0, 0 },tmp);
	stork_idle = load_animation("img/new_a/idle_ss_512_512_is_10_12.png", { 0, 0 },tmp);
	stork_jump_run = load_animation("img/new_a/jump_run_short_ss_512_512_is_10_10.png", { 0, 0 },tmp);
	stork_jump_idle = load_animation("img/new_a/jump_idle_short_ss_512_512_is_10_10.png", { 0, 0 },tmp);
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