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

Dynamic_animation* Assets::load_dynamic_animation(std::string path)
{
	std::ifstream f(path);
	int frames, parts;
	f >> parts >> frames;
	std::vector<std::vector<float>> kf(frames, std::vector<float>((size_t)parts + 3));
	std::vector<int> l(frames);
	for (int i = 0; i < frames; i++)
	{
		for (int j = 0; j < parts + 3; j++)
		{
			f >> kf[i][j];
		}
	}
	for (int i = 0; i < frames; i++)
		f >> l[i];
	bool repeat;
	f >> repeat;
	return new Dynamic_animation(kf, l, repeat);
}

Animation_tree::Animation_tree(int _count, int i_count) : count(_count), independent_count(i_count)
{
	root = 0;
	tree.resize(count);
	position_of_element_in_animation_array.resize(count);
	nodes.resize(count);
}

Animation_tree Assets::load_animation_tree(std::string path)
{
	std::ifstream file;
	file.open(path);
	int count, independent_count;
	file >> count >> independent_count;
	Animation_tree tree(count, independent_count);
	std::map<std::string, int> node_names;
	for (int i = 0; i < count; i++)
	{
		std::string name;
		int position;
		file >> name >> position;
		if (position == -1)
		{
			tree.root = i;
		}
		node_names[name] = i;
		tree.position_of_element_in_animation_array[node_names[name]] = position;
	}
	for (int i = 0; i < count - 1; i++)
	{
		std::string a, b;
		int ax, ay, bx, by;
		file >> a >> b;
		file >> ax >> ay >> bx >> by;
		tree.nodes[node_names[b]].delta_pos = { Vectori(ax,ay),Vectori(bx,by) };
		tree.tree[node_names[a]].push_back(node_names[b]);
	}
	tree.nodes[tree.root].delta_pos = { Vectori(0,0),Vectori(0,0) };
	return tree;
}

void Assets::load_textures(std::vector<sf::Texture>& v, std::string path, bool rep)
{
	int a, b, c, d;
	int l = (int)path.rfind("_ss_");
	if (l == std::string::npos)
	{
		l = -1;
	}
	l++;
	size_t r = path.find(".", l);
	std::string tmp = path.substr(l, r - l);
	for (auto& it : tmp)
	{
		if (it > '9' || it < '0')
			it = ' ';

	}
	std::stringstream tmps(tmp);
	if (!(tmps >> a >> b >> c >> d))
	{
		std::cerr << "error reading sizes " + path << std::endl;
		return;
	}
	v.reserve((uint64_t)c * d + 50);
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		std::cerr << "Error while loading" + path << std::endl;
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

void Assets::load_shaders()
{
	blurh.loadFromFile("img/shaders/blur_h.frag", sf::Shader::Fragment);
	blurv.loadFromFile("img/shaders/blur_v.frag", sf::Shader::Fragment);
	blurh.setUniform("sigma", 50.0f);
	blurh.setUniform("blurSize", 1.0f / context.resolution.x);
	blurh.setUniform("blurSampler", sf::Shader::CurrentTexture);
	blurv.setUniform("sigma", 50.0f);
	blurv.setUniform("blurSize", 1.0f / context.resolution.y);
	blurv.setUniform("blurSampler", sf::Shader::CurrentTexture);
	white.loadFromFile("img/shaders/white.frag", sf::Shader::Fragment);
	context.global.loadFromFile("img/shaders/global.frag", sf::Shader::Fragment);
	context.global.setUniform("light", sf::Shader::CurrentTexture);
}

void Assets::load_additional_texture(std::string path, std::string name, int repeat)
{
	map_textures.push_back(sf::Texture());
	map_textures.back().loadFromFile(path);
	if (repeat == 1)
	{
		map_textures.back().setRepeated(true);
	}
	textures[name] = &map_textures.back();
}

void Assets::load_assets()
{
	sf::FloatRect tmp(210, 100, 105, 340);
	pieces = new sf::Texture();
	pieces->loadFromFile("img/stork/parts_ss_128_128_is_3_9.png");
	for (int i = 0; i < 27; i++)
		pieces_rect.push_back({ 128 * (i % 3),128 * (i / 3),128,128 });
	bg = new sf::Texture();
	bg->loadFromFile("img/bg/bg.jpg");
	layer2 = new sf::Texture();
	layer2->loadFromFile("img/bg/LAS.png");
	light = new sf::Texture();
	light->loadFromFile("img/light.png");
	console_bg = new sf::Texture();
	console_bg->loadFromFile("img/console_bg.png");
	animations.push_back(load_dynamic_animation("animations/stork/idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run2.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/punch1.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/punch2.txt"));
	load_textures(map_textures, "img/tex_ss_64_64_is_6_7.png", true);
	stork_tree = load_animation_tree("animations/stork/tree.txt");
	load_shaders();

	textures["asphalt,0"] = &map_textures[0];
	textures["concrete,0"] = &map_textures[1];
	textures["construction,0"] = &map_textures[2];
	textures["bricks,0"] = &map_textures[3];
	textures["bricks,1"] = &map_textures[4];
	textures["bricks,2"] = &map_textures[5];
	textures["bricks,3"] = &map_textures[6];
	textures["tile,0"] = &map_textures[7];
	textures["rivets,0"] = &map_textures[8];
	textures["panels,0"] = &map_textures[9];
	textures["panels,1"] = &map_textures[10];
	textures["panels,2"] = &map_textures[11];
	textures["grass,0"] = &map_textures[12];
	textures["ribbing,0"] = &map_textures[13];
	textures["ribbing,1"] = &map_textures[14];
	textures["ribbing,2"] = &map_textures[15];
	textures["dirt,0"] = &map_textures[16];
	textures["paving,0"] = &map_textures[17];
	textures["paving,1"] = &map_textures[18];
	textures["wood,0"] = &map_textures[19];
	textures["wood,1"] = &map_textures[20];
	textures["wood,2"] = &map_textures[21];
	textures["wood,3"] = &map_textures[22];
	textures["wood,4"] = &map_textures[23];
	textures["tile1"] = &map_textures[24];
	textures["canal0"] = &map_textures[25];
	textures["canal1"] = &map_textures[26];
	textures["krata0"] = &map_textures[27];
	textures["wall0"] = &map_textures[28];
	textures["pipes0"] = &map_textures[29];
	textures["pipes1"] = &map_textures[30];
	textures["pipes2"] = &map_textures[31];
	textures["pipes3"] = &map_textures[32];
	textures["tapeta0"] = &map_textures[33];
	textures["tapeta1"] = &map_textures[34];
	textures["tapeta2"] = &map_textures[35];
	textures["tapeta3"] = &map_textures[36];
	textures["vent0"] = &map_textures[37];
	textures["vent1"] = &map_textures[38];

	context.blurh_states.shader = &blurh;
	context.blurv_states.shader = &blurv;
	context.final_states.blendMode = sf::BlendMultiply;
	context.white_states.shader = &white;

	storkfont.loadFromFile("StorkFont.ttf");
	consola.loadFromFile("consola.ttf");
}