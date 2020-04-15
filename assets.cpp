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
	int frames, parts;
	f >> parts >> frames;
	std::vector<std::vector<float>> kf(frames, std::vector<float>(parts + 3));
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
		tree.nodes[node_names[b]].delta_pos = { Vectorf(ax,ay),Vectorf(bx,by) };
		tree.tree[node_names[a]].push_back(node_names[b]);
	}
	tree.nodes[tree.root].delta_pos = { Vectorf(0,0),Vectorf(0,0) };
	return tree;
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
		pieces_rect.push_back({ 128 * (i % 3),128 * (i / 3),128,128 });
	bg = new sf::Texture();
	bg->loadFromFile("img/bg.jpg");
	animations.push_back(load_dynamic_animation("animations/stork/idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_idle.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/jump_run2.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/punch1.txt"));
	animations.push_back(load_dynamic_animation("animations/stork/punch2.txt"));
	load_textures(map_textures, "img/tex_ss_64_64_is_1_17.png", true);
	stork_tree = load_animation_tree("animations/stork/tree.txt");
	context.generate_map.loadFromFile("img/shaders/generate_map.frag", sf::Shader::Fragment);
	context.black.loadFromFile("img/shaders/black.frag", sf::Shader::Fragment);
	context.blurh.loadFromFile("img/shaders/blur_h.frag", sf::Shader::Fragment);
	context.blurv.loadFromFile("img/shaders/blur_v.frag", sf::Shader::Fragment);
	context.generate_map.setUniform("texture", sf::Shader::CurrentTexture);
	context.generate_map.setUniform("samples", 0.01f);
	context.generate_map.setUniform("dimensions", context.resolution);
	context.blurh.setUniform("sigma", 20.0f);
	context.blurh.setUniform("blurSize", 1.0f / context.resolution.x);
	context.blurh.setUniform("blurSampler", sf::Shader::CurrentTexture);
	context.blurv.setUniform("sigma", 20.0f);
	context.blurv.setUniform("blurSize", 1.0f / context.resolution.y);
	context.blurv.setUniform("blurSampler", sf::Shader::CurrentTexture);
	//load_textures(ship_dockx, "img/ships/DokowanieX_ss_436_87_is_10_12.png", false);
	//load_textures(ship_docky, "img/ships/DokowanieY_ss_443_442_is_15_20.png", false);
	//load_textures(ship_fly, "img/ships/Lot_ss_466_87_is_6_10.png", false);
	//load_textures(ship_idle, "img/ships/Postój_ss_446_87_is_6_10.png", false);
	//load_textures(ship_ext_hdmi, "img/ships/Wysuniêcie HDMI_ss_446_209_is_6_10.png", false);
	//load_textures(ship_ret_hdmi, "img/ships/Schowanie HDMI_ss_446_209_is_6_10.png", false);
	//load_textures(ship_undockx, "img/ships/WydokowanieX_ss_436_87_is_6_10.png", false);
	//load_textures(ship_undocky, "img/ships/WydokowanieY_ss_443_442_is_10_12.png", false);

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

	std::cout << "done!" << std::endl;
}