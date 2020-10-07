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
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			if (!a[j * x + i].loadFromImage(img, sf::IntRect(i * sx, j * sy, sx, sy)))
			{
				context.console->err << "animation loading error" << "\n";
				return;
			}
		}
	}
}

Dynamic_animation_struct* Assets::load_dynamic_animation(std::string path)
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
	return new Dynamic_animation_struct(kf, l, repeat);
}

const Animation_tree Assets::load_animation_tree(std::string path)
{
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.is_open())
	{
		throw std::runtime_error("Can't open file!");
	}
	std::stringstream file = util::remove_comments(file_raw);
	file_raw.close();
	int count, independent_count;
	Vectori part_sizes, frame_size, offset;
	file >> count >> independent_count >> part_sizes.x >> part_sizes.y;
	file >> frame_size.x >> frame_size.y >> offset.x >> offset.y;
	Frame_info info(part_sizes, frame_size, offset);
	Animation_tree tree(count, independent_count, info);
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
	while (!file.eof())
	{
		string line;
		std::getline(file, line);
		if (std::all_of(line.begin(), line.end(), [](char c) {return static_cast<bool>(std::isspace(c)); }))
		{
			continue;
		}
		std::stringstream ss = std::stringstream(line);
		std::pair<int, int> transition;
		int temp;
		Animation_index alternative;
		try
		{
			ss >> transition.first >> transition.second >> temp;
			alternative = static_cast<Animation_index>(temp);
		}
		catch (...)
		{
			throw std::invalid_argument("Error in animation tree!");
		}
		string s;
		std::vector<int> keys;
		ss >> s;
		if (s != "(")
		{
			throw std::invalid_argument("Error in animation tree!");
		}
		while (ss >> s)
		{
			try
			{
				if (s != ")")
				{
					keys.push_back(std::stoi(s));
				}
				else
				{
					break;
				}
			}
			catch (...)
			{
				throw std::invalid_argument("Error in animation tree!");
			}
		}
		tree.alternative_animations.insert({ transition, std::make_pair(alternative, keys) });
	}
	return tree;
}

void Assets::load_sound(sf::SoundBuffer& buf, string path)
{
	if (!buf.loadFromFile(path))
	{
		context.console->err << "Sound loading error" << "\n";
		return;
	}
}

void Assets::load_sounds()
{
	sounds.resize(3);
	load_sound(sounds[0], "sound/aaa.wav");
	load_sound(sounds[1], "sound/jump_idle.wav");
	load_sound(sounds[2], "sound/jump_run.wav");
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
	v.reserve((uint64_t)c * d + MAX_ADDITIONAL_TEXTURES);
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
	load_animation(animations[name], im, n.x, n.y, size.x, size.y);
}

std::vector<const Dynamic_animation_struct*>* Assets::load_dynamic_animations(std::vector<string> paths)
{
	Dynamic_animation_struct* default_animation = load_dynamic_animation("animations/stork/idle.txt");
	dynamic_animations.push_back(default_animation);
	for (int i = 1; i < paths.size(); i++)
	{
		if (paths[i] == "-")
		{
			dynamic_animations.push_back(default_animation);
		}
		else
		{
			dynamic_animations.push_back(load_dynamic_animation(paths[i]));
		}
	}
	return &dynamic_animations;
}

void Assets::load_hp_bar()
{
	hp_bar.top = std::make_shared<sf::Texture>();
	hp_bar.top->loadFromFile("img/ui/bar_top.png");
	hp_bar.mid = std::make_shared<sf::Texture>();
	hp_bar.mid->loadFromFile("img/ui/bar_mid.png");
	hp_bar.bot = std::make_shared<sf::Texture>();
	hp_bar.bot->loadFromFile("img/ui/bar_bot.png");
	hp_bar.content_top = std::make_shared<std::vector<sf::Texture>>();
	hp_bar.content_top->resize(16);
	hp_bar.content_mid = std::make_shared<std::vector<sf::Texture>>();
	hp_bar.content_mid->resize(16);
	hp_bar.content_bot = std::make_shared<std::vector<sf::Texture>>();
	hp_bar.content_bot->resize(16);
	sf::Image im;
	im.loadFromFile("img/ui/bar_content.png");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*hp_bar.content_top)[i * 4 + j].loadFromImage(im, sf::IntRect(j * 128, i * 128, 128, 47));
			(*hp_bar.content_mid)[i * 4 + j].loadFromImage(im, sf::IntRect(j * 128, i * 128 + 47, 128, 34));
			(*hp_bar.content_bot)[i * 4 + j].loadFromImage(im, sf::IntRect(j * 128, i * 128 + 47 + 34, 128, 47));
		}
	}
}

void Assets::load_assets()
{
	//Storkman
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
	//User interface
	console_bg = new sf::Texture();
	console_bg->loadFromFile("img/ui/console_bg.png");
	load_hp_bar();
	//Textures
	enemy_textures.reserve(20);
	enemy_textures.push_back(sf::Texture());
	load_texture(enemy_textures.back(), "img/enemy/KLODA_00.png", false);
	enemy_textures.push_back(sf::Texture());
	load_texture(enemy_textures.back(), "img/enemy/SZNUR_00.png", false);
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
	{
		std::vector<string> tex_names = { "log,1","line,1" };
		for (int i = 0; i < tex_names.size(); i++)
		{
			textures[tex_names[i]] = &enemy_textures[i];
		}
	}
	//Icon
	icon.loadFromFile("img/ikona.png");
	//Fonts
	storkfont.loadFromFile("data/fonts/StorkFont.ttf");
	consola.loadFromFile("data/fonts/consola.ttf");
	//Sounds and music
	load_sounds();
}

void Assets::parse_additional_textures(string path)
{
	std::ifstream file;
	string p, name;
	int repeat;

	file.open(path);
	if (file.good())
	{
		while (!file.eof())
		{
			file >> p >> name >> repeat;
			load_additional_texture(p, name, repeat);
		}
	}
}

void Assets::parse_additional_animations(string path)
{
	std::ifstream file;
	string p, name;
	int x, y, sx, sy;

	file.open(path);
	if (file.good())
	{
		while (!file.eof())
		{
			file >> p >> name >> x >> y >> sx >> sy;
			load_additional_animation(p, name, Vectori(x, y), Vectori(sx, sy));
		}
	}
}