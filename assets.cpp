#include "assets.h"

void Assets::load_texture(sf::Texture& t, sf::Image& img, int x, int y, int sx, int sy, bool rep)
{
	if (!t.loadFromImage(img, sf::IntRect(x, y, sx, sy)))
	{
		context.console->err << "Texture loading error" << "\n";
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

void Assets::load_texture(sf::Texture& t, string path, bool rep)
{
	if (!t.loadFromFile(path))
	{
		context.console->err << "Texture loading error" << "\n";
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

void Assets::load_animation(std::vector<sf::Texture>& a, sf::Image& img, int x,
	int y, int sx, int sy)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (!a[i * x + j].loadFromImage(img, sf::IntRect(i * sx, j * sy, sx, sy)))
			{
				context.console->err << "animation loading error" << "\n";
				return;
			}
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
		context.console->err << "error reading sizes " + path << "\n";
		return;
	}
	v.reserve((uint64_t)c * d + 50);
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		context.console->err << "Error while loading" + path << "\n";
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

void Assets::load_additional_texture(std::string path, std::string name,
	int repeat)
{
	map_textures.push_back(sf::Texture());
	map_textures.back().loadFromFile(path);
	if (repeat == 1)
	{
		map_textures.back().setRepeated(true);
	}
	textures[name] = &map_textures.back();
}

void Assets::load_additional_animation(string path, string name, Vectori n,
	Vectori size)
{
	sf::Image im;
	im.loadFromFile(path);
	animations[name] = std::vector<sf::Texture>(n.x * n.y);
	load_animation(animations[name], im, n.y, n.x, size.x, size.y);
}

void Assets::load_assets()
{
	//Storkman
	sf::FloatRect tmp(210, 100, 105, 340);
	pieces = new sf::Texture();
	pieces->loadFromFile("img/stork/parts_ss_128_128_is_3_9.png");
	for (int i = 0; i < 27; i++)
		pieces_rect.push_back({ 128 * (i % 3),128 * (i / 3),128,128 });
	//Background
	bg = new sf::Texture();
	bg->loadFromFile("img/bg/bg.jpg");
	layer2 = new sf::Texture();
	layer2->loadFromFile("img/bg/LAS.png");
	light = new sf::Texture();
	light->loadFromFile("img/light.png");
	//Console
	console_bg = new sf::Texture();
	console_bg->loadFromFile("img/console_bg.png");
	//Dynamic animations
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/idle.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/run.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/jump_idle.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/jump_run.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/jump_run2.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/punch1.txt"));
	dynamic_animations.push_back(load_dynamic_animation("animations/stork/punch2.txt"));
	stork_tree = load_animation_tree("animations/stork/tree.txt");
	//Textures
	enemy_textures.reserve(20);
	enemy_textures.push_back(sf::Texture());
	load_texture(enemy_textures.back(), "img/enemy/KLODA_00.PNG", false);
	enemy_textures.push_back(sf::Texture());
	load_texture(enemy_textures.back(), "img/enemy/SZNUR_00.PNG", false);
	load_textures(map_textures, "img/tex_ss_64_64_is_6_7.png", true);
	{
		std::vector<string> tex_names = { "asphalt,0","concrete,0","construction,0",
		"bricks,0","bricks,1","bricks,2", "bricks,3","tile,0","rivets,0",
		"panels,0","panels,1","panels,2","grass,0","ribbing,0","ribbing,1",
		"ribbing,2","dirt,0","paving,0","paving,1","wood,0","wood,1","wood,2",
		"wood,3","wood,4","tile1","canal0","canal1","krata0","wall0","pipes0",
		"pipes1","pipes2","pipes3","tapeta0","tapeta1","tapeta2","tapeta3",
		"vent0","vent1" };
		for (int i = 0; i < tex_names.size(); i++)
		{
			textures[tex_names[i]] = &map_textures[i];
		}
	}

	//Shaders
	load_shaders();
	context.blurh_states.shader = &blurh;
	context.blurv_states.shader = &blurv;
	context.final_states.blendMode = sf::BlendMultiply;
	context.white_states.shader = &white;
	//Icon
	icon.loadFromFile("img/ikona.png");
	//Fonts
	storkfont.loadFromFile("StorkFont.ttf");
	consola.loadFromFile("consola.ttf");
}