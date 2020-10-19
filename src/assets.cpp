#include "assets.h"

const sf::Texture* Assets::load_texture(sf::Image& img, Vectori pos, Vectori size, bool repeat)
{
	sf::Texture* tex = new sf::Texture();
	if (!tex->loadFromImage(img, sf::IntRect(pos, size)))
	{
		context.console->err << "Texture loading error" << "\n";
		throw std::invalid_argument("Error while loading texture");
	}
	else
	{
		tex->setRepeated(repeat);
	}
	return tex;
}

const sf::Texture* Assets::load_texture(string path, bool repeat)
{
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		context.console->err << "Error while loading" + path << "\n";
		throw std::invalid_argument("Error while loading texture");
	}
	return load_texture(image, { 0,0 }, static_cast<Vectori>(image.getSize()), repeat);
}

std::vector<const sf::Texture*> Assets::load_textures(sf::Image& img, Vectori frames,
	Vectori size, bool repeat)
{
	std::vector<const sf::Texture*> temp_textures;
	for (int j = 0; j < frames.y; j++)
	{
		for (int i = 0; i < frames.x; i++)
		{
			sf::Texture* tmp = new sf::Texture();
			if (!tmp->loadFromImage(img, sf::IntRect(i * size.x, j * size.y, size.x, size.y)))
			{
				context.console->err << "animation loading error" << "\n";
				throw std::invalid_argument("Error while loading textures");
			}
			tmp->setRepeated(repeat);
			temp_textures.push_back(tmp);
		}
	}
	return temp_textures;
}

std::vector<const sf::Texture*> Assets::load_textures(std::string path, Vectori n,
	Vectori size, bool repeat)
{
	sf::Image image;
	if (!image.loadFromFile(path))
	{
		context.console->err << "Error while loading" + path << "\n";
		throw std::invalid_argument("Error while loading textures");
	}
	return load_textures(image, n, size, repeat);
}

Dynamic_animation_struct* Assets::load_dynamic_animation(std::string path)
{
	std::ifstream f(path);
	int frames, parts;
	f >> parts >> frames;
	std::vector<std::vector<float>> kf(frames, std::vector<float>((size_t)parts + 3));
	std::vector<float> l(frames);
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

std::tuple<Vectori, Vectori, std::vector<string>> Assets::load_texture_file_config(string path)
{
	std::vector<string> names;
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::runtime_error("Can't open file: " + path);
	}
	std::stringstream file = util::remove_comments(file_raw);
	Vectori textures_n, size;
	file >> textures_n.x >> textures_n.y >> size.x >> size.y;
	string s;
	while (file >> s)
	{
		names.push_back(s);
	}
	return std::make_tuple(textures_n, size, names);
}

void Assets::load_assets()
{
	//Storkman

	//Background
	backgrounds["main_bg"] = load_texture("img/bg/bg.jpg", false);
	backgrounds["forest_bg"] = load_texture("img/bg/LAS.png", false);
	//User interface
	console_bg = load_texture("img/ui/console_bg.png", false);
	load_hp_bar();
	//Textures
	auto [n, size, tex_names] = load_texture_file_config("img/world_textures_config.txt");
	std::vector<const sf::Texture*> tmp = load_textures("img/tex/world_textures.png", n, size, true);
	for (int i = 0; i < tex_names.size(); i++)
	{
		textures[tex_names[i]] = tmp[i];
	}
	parse_additional_textures("img/textures.txt");
	parse_additional_animations("img/animations.txt");
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
	string p, name;
	int repeat;
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::runtime_error("Can't open file: " + path);
	}
	std::stringstream file = util::remove_comments(file_raw);
	while (!file.eof())
	{
		file >> p >> name >> repeat;
		textures[name] = load_texture(p, repeat);
	}
}

void Assets::parse_additional_animations(string path)
{
	string p, name;
	Vectori n, size;
	bool repeat;
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::runtime_error("Can't open file: " + path);
	}
	std::stringstream file = util::remove_comments(file_raw);
	while (!file.eof())
	{
		file >> p >> name >> n.x >> n.y >> size.x >> size.y >> repeat;
		animations[name] = load_textures(p, n, size, repeat);
	}
}

std::vector<std::vector<const sf::Texture*>> Assets::load_entity_textures(
	std::vector<string> paths, Vectori textures_n, Vectori texture_size)
{
	std::vector<std::vector<const sf::Texture*>> temp;
	for (const auto& it : paths)
	{
		temp.push_back(load_textures(it, textures_n, texture_size, false));
	}
	return temp;
}
