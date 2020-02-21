#include "assets.h"
#include<sstream>
void Assets::load_texture(sf::Texture& t, sf::Image img, int y, int x, int sx, int sy, bool rep)
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

void Assets::load_textures(std::vector<sf::Texture>& v, std::string path, bool rep)
{
	int a, b, c, d;
	int l=path.rfind("/");
	if (l == std::string::npos)
	{
		l = -1;
	}
	l++;
	int r = path.find(".", l);
	std::string tmp = path.substr(l, r - l);
	for (auto &it : tmp)
	{
		if (it>'9' || it<'0')
			it = ' ';

	}
	std::stringstream tmps(tmp);
	if(!(tmps >> a >> b >> c >> d))
	//if (4 != sscanf_s(path.c_str(), "_ss_%d_%d_is_%d_%d", &a, &b, &c, &d))
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
	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			v.push_back(sf::Texture());
			load_texture(v.back(), image, j * a, i * b, a, b, rep);
		}
	}
}

void Assets::load_assets()//const std::string names[])
{
	load_textures(stork_run, "img/stork/run_ss_347_358_is_8_10.png", false);
	load_textures(stork_idle, "img/stork/idle_ss_158_356_is_10_12.png", false);
	load_textures(map_textures, "img/tex_ss_64_64_is_1_17.png", true);
	textures["bricks,0"] = std::make_shared<sf::Texture>(map_textures[3]);
	textures["bricks,1"] = std::make_shared<sf::Texture>(map_textures[4]);
	textures["bricks,2"] = std::make_shared<sf::Texture>(map_textures[5]);
	textures["bricks,3"] = std::make_shared<sf::Texture>(map_textures[6]);
	/*
	bricks.reserve(4);	//sf::Texture nie mo¿e byæ kopiowane
	for (int i = 0; i < 4; i++)
	{
		bricks.push_back(sf::Texture());
		if (!bricks[i].loadFromFile("img/tex/CEGLY0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error CEGLY0" + std::to_string(i) + ".png" << std::endl;
		}
		bricks[i].setRepeated(true);
	}
	panels.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		panels.push_back(sf::Texture());
		if (!panels[i].loadFromFile("img/tex/PANELE0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error PANELE0" + std::to_string(i) + ".png" << std::endl;
		}
		panels[i].setRepeated(true);
	}
	metal.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		metal.push_back(sf::Texture());
		if (!metal[i].loadFromFile("img/tex/ZEBROWANIE0" + std::to_string(i) + ".PNG"))
		{
			std::cout << "Error ZEBROWANIE0" + std::to_string(i) + ".png" << std::endl;
		}
		metal[i].setRepeated(true);
	}
	//load_texture(asphalt, "img/tex/ASFALT.png");
	asphalt.setRepeated(true);
	if (!concrete.loadFromFile("img/tex/BETON.png"))
	{
		std::cout << "Error BETON.png" << std::endl;
	}
	concrete.setRepeated(true);
	if (!ground.loadFromFile("img/tex/ZIEMIA.png"))
	{
		std::cout << "Error ZIEMIA.png" << std::endl;
	}
	ground.setRepeated(true);
	if (!grass.loadFromFile("img/tex/TRAWA.png"))
	{
		std::cout << "Error TRAWA.png" << std::endl;
	}
	grass.setRepeated(true);
	if (!rivet.loadFromFile("img/tex/NITY00.png"))
	{
		std::cout << "Error NITY00.png" << std::endl;
	}
	rivet.setRepeated(true);
	stork_run.reserve(80);
	sf::Image image;
	if (!image.loadFromFile("img/stork/run_ss_347_358_is_8_10.png"))
	{
		std::cout << "Stork run texture error" << std::endl;
	}
	for(int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			stork_run.push_back(sf::Texture());
			load_texture(stork_run.back(), image, j * 347, i * 358, 347, 358, false);
		}
	}
		*/
}