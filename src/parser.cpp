#include "parser.h"

void Parser::parse_surface_types_config(string path)
{
	std::ifstream config_file(path, std::ios::in);
	std::stringstream config_file_stripped = util::remove_comments(config_file);
	string tmp;
	while (config_file_stripped >> tmp)
	{
		name_to_surface[tmp] = name_to_surface.size();
	}
}

Parser::Parser(Assets* _assets) : assets(_assets)
{};

std::pair<Vectori, Vectori> Parser::parse_map_element(tinyxml2::XMLElement* element)
{
	if (element == nullptr)
	{
		std::cout << "File error - corrupted XML" << '\n';
		throw std::invalid_argument("NULL pointer");
	}
	string root_name = element->Name();
	if (root_name != "map")
	{
		std::cout << "No map element" << '\n';
		throw std::invalid_argument("No Map node");
	}
	Vectori map_player_pos, map_size;
	try
	{
		map_player_pos = get_and_parse_var<Vectori>("player_pos", element);
		map_size = get_and_parse_var<Vectori>("size", element);
	}
	catch (...)
	{
		std::cout << "Error in map definition" << '\n';
		throw std::invalid_argument("Invalid map attributes");
	}
	if (map_player_pos.x < 0 || map_player_pos.y < 0)
	{
		throw std::invalid_argument("Negative player pos");
	}
	if (map_size.x < 0 || map_size.y < 0)
	{
		throw std::invalid_argument("Negative map size");
	}
	return std::make_pair(map_size, map_player_pos);
}

std::tuple<Vectori, string, string> Parser::parse_level_element(tinyxml2::XMLElement* element, Vectori map_size)
{
	Vectori pos;
	string filepath;
	pos = get_and_parse_var<Vectori>("position", element);
	if (pos.x < 0 || pos.y < 0)
	{
		throw std::invalid_argument("Negative level pos");
	}
	if (pos.x >= map_size.x || pos.y >= map_size.y)
	{
		throw std::invalid_argument("Level outside of map size");
	}
	filepath = get_attribute_by_name("filename", element);
	string code = get_attribute_by_name("code", element);
	std::vector<string> s = split_string("a,b,c,d,e");
	return std::make_tuple(pos, filepath, code);
}

Level Parser::parse_level(tinyxml2::XMLElement* root, Vectori global_pos, int code)
{
	if (root == nullptr)
	{
		std::cout << "Error in first level element" << '\n';
		throw std::invalid_argument("NULL pointer in level file");
	}
	string root_name = root->Name();
	if (root_name != "level")
	{
		std::cout << "No level element" << '\n';
		throw std::invalid_argument("No level node");
	}
	tinyxml2::XMLElement* element = root->FirstChildElement();
	std::vector<std::unique_ptr<Chunk>> chunks;
	std::vector<Map_sound> sounds;
	int sound_id = 0;
	int event_index = 1;
	while (element != nullptr)
	{
		string name = element->Name();
		if (name == "chunk")
		{
			chunks.emplace_back(std::move(parse_chunk(element, global_pos)));
		}
		if (name == "dynamic_chunk")
		{
			chunks.emplace_back(std::move(parse_dynamic_chunk(element, global_pos)));
		}
		if (name == "event")
		{
			events.insert({ get_attribute_by_name("name", element), event_index++ });
		}
		if (name == "sound")
		{
			sounds.push_back(parse_sound(element, global_pos, sound_id));
			sound_id++;
		}
		element = element->NextSiblingElement();
	}
	return Level(std::move(chunks),
		std::move(sounds), global_pos, code);
}

std::unique_ptr<Level> Parser::open_and_parse_level(Vectori pos, string filepath, int code)
{
	tinyxml2::XMLDocument lvl_root;
	lvl_root.LoadFile(filepath.c_str());
	return std::make_unique<Level>(parse_level(lvl_root.FirstChildElement(), pos, code));
}

std::unique_ptr<Chunk> Parser::parse_chunk(tinyxml2::XMLElement* root, Vectori level_pos)
{
	std::vector<std::shared_ptr<Physical_updatable>> p_updatables;
	std::vector<std::shared_ptr<Graphical_updatable>> g_updatables;
	std::vector<std::pair<int, std::shared_ptr<Renderable>>> drawables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	std::vector<std::shared_ptr<Zone>> zones;
	std::vector<std::shared_ptr<Map_object>> map_objects;
	std::vector<sf::Vertex> collision_vertices;
	std::vector<std::shared_ptr<Interactive>> interactives;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	while (element != nullptr)
	{
		string name = element->Name();
		auto [layer, ptr] = element_name_to_function_returning_pair.at(name)(element, level_pos);
		map_objects.push_back(ptr);
		auto drawable = dynamic_pointer_cast<Renderable>(ptr);
		if (drawable)
		{
			drawables.emplace_back(layer.value(), drawable);
		}
		auto p_updatable = dynamic_pointer_cast<Physical_updatable>(ptr);
		if (p_updatable)
		{
			p_updatables.push_back(p_updatable);
		}
		auto g_updatable = dynamic_pointer_cast<Graphical_updatable>(ptr);
		if (g_updatable)
		{
			g_updatables.push_back(g_updatable);
		}
		auto collidable = dynamic_pointer_cast<Collidable>(ptr);
		if (collidable)
		{
			collidables.push_back(collidable);
			if (!p_updatable)
			{
				add_vertices(collision_vertices, collidable->get_collision());
			}
		}
		auto zone = dynamic_pointer_cast<Zone>(ptr);
		if (zone)
		{
			zones.push_back(zone);
		}
		auto inter = dynamic_pointer_cast<Interactive>(ptr);
		if (inter)
		{
			interactives.push_back(inter);
		}
		element = element->NextSiblingElement();
	}
	sf::FloatRect bound = calculate_chunk_bounds(root, map_objects);
	sf::VertexBuffer collision_buffer(sf::Lines, sf::VertexBuffer::Static);
	collision_buffer.create(collision_vertices.size());
	collision_buffer.update(collision_vertices.data());
	return std::make_unique<Map_chunk>(std::move(p_updatables),
		std::move(g_updatables), std::move(drawables), std::move(collidables),
		std::move(zones), std::move(interactives), bound, 
		std::move(collision_buffer));

}

std::unique_ptr<Chunk> Parser::parse_dynamic_chunk(tinyxml2::XMLElement* root, Vectori level_pos)
{
	std::vector<std::unique_ptr<Chunk>> chunks;
	std::unordered_map<int, int> transition_array;
	std::unordered_map<int, std::vector<std::pair<float, int>>> time_events;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	int chunk_index = 0;
	while (element != nullptr)
	{
		string name = element->Name();
		if (name == "state_trigger")
		{
			string trigger_val = get_attribute_by_name("trigger", element);
			if (trigger_val == "")
			{
				trigger_val = "default";
			}
			int state = get_and_parse_var<int>("state", element);
			transition_array.insert({ events.at(trigger_val), state });
		}
		if (name == "timed_event")
		{
			string trigger_val = get_attribute_by_name("event", element);
			int state = get_and_parse_var<int>("state", element);
			float time = get_and_parse_var<float>("time", element);
			auto pair = std::make_pair(time, events.at(trigger_val));
			if (!time_events.contains(state))
			{
				time_events[state] = std::vector <std::pair<float, int>>();
			}
			time_events[state].push_back(pair);
		}
		if (name == "state")
		{
			chunks.emplace_back(std::move(parse_chunk(element, level_pos)));
		}
		element = element->NextSiblingElement();
	}
	return std::make_unique<Dynamic_chunk>(std::move(chunks),
		std::move(transition_array), std::move(time_events));
}

std::pair<std::optional<int>, std::shared_ptr<Platform>>
Parser::parse_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		bool one_sided = get_and_parse_var<bool>("one_sided", element, false);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		return std::make_pair(layer, std::make_shared<Platform>(pos, tex, points, surface, one_sided, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "platform" << '\n';
		std::cout << "Check vertices and surface type" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "platform" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Platform error");
}

std::pair<std::optional<int>, std::shared_ptr<Textured_polygon>>
Parser::parse_wall(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_WALL_LAYER);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		return std::make_pair(layer, std::make_shared<Textured_polygon>(pos, tex, points, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "wall" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "wall" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<std::optional<int>, std::shared_ptr<Object>>
Parser::parse_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		return std::make_pair(layer, std::make_shared<Object>(pos, tex, height,
			fliprot.first, fliprot.second, color));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Object error");
}

std::pair<std::optional<int>, std::shared_ptr<Animated_object>>
Parser::parse_animated_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		string val = get_attribute_by_name("texture", element);
		const std::vector<const sf::Texture*>* tex = &assets->animations.at(val);
		float height = get_and_parse_var <float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		return std::make_pair(layer, std::make_shared<Animated_object>(pos, std::move(animation),
			height, fliprot.first, fliprot.second, color));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Check animation" << '\n';
	}
	throw std::runtime_error("Animated object error");
}

std::pair<std::optional<int>, std::shared_ptr<Moving_animated_object>>
Parser::parse_animated_moving_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		string val = get_attribute_by_name("texture", element);
		const std::vector<const sf::Texture*>* tex = &assets->animations.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		std::unique_ptr<Simple_AI> ai;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_animated_object>(pos, std::move(animation),
			height, std::move(ai), fliprot.first, fliprot.second, color));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Check animation" << '\n';
	}
	throw std::runtime_error("Animated object error");
}

Map* Parser::parse_map(tinyxml2::XMLElement* root)
{
	auto map_data = parse_map_element(root);
	Vectori map_size = map_data.first;
	Vectori player_pos = map_data.second;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	Map* map = new Map(map_size, player_pos, assets->backgrounds.at("main_bg"));
	int lvl_n = 0;
#define DEBUG_ASYNC__STORK__PARSE
#ifndef DEBUG_ASYNC__STORK__PARSE
	std::vector<std::future<std::unique_ptr<Level>>> futures;
	while (element != NULL)
	{
		string name = element->Name();
		if (name == "level")
		{
			auto [pos, path, code] = parse_level_element(element, map_size);
			level_names[code] = lvl_n;
			if (level_music.contains(code))
			{
				music_paths.push_back(level_music[code]);
			}
			else
			{
				music_paths.push_back(level_music["default"]);
			}
			futures.push_back(std::async(std::launch::async, &Parser::open_and_parse_level, this, pos, path, lvl_n));
			lvl_n++;
		}
		else
		{
			throw std::invalid_argument("Incorrect element in map file");
		}
		element = element->NextSiblingElement();
	}
	for (auto& it : futures)
	{
		map->add_level(std::move(it.get()));
	}
#else
	while (element != NULL)
	{
		string name = element->Name();
		if (name == "level")
		{
			auto [pos, path, code] = parse_level_element(element, map_size);
			level_names[code] = lvl_n;
			map->add_level(std::move(open_and_parse_level(pos, path, lvl_n)));
			lvl_n++;
		}
		else
		{
			throw std::invalid_argument("Incorrect element in map file");
		}
		element = element->NextSiblingElement();
	}
#endif

	return map;
}

Entity_config Parser::parse_entity_config(string path)
{
	std::ifstream raw_file(path);
	if (!raw_file.is_open())
	{
		throw std::runtime_error("Can't open entity config file!");
	}
	std::stringstream file = util::remove_comments(raw_file);
	Entity_config config;
	file >> config.type;
	file >> config.tree_file;
	file >> config.textures_n.x >> config.textures_n.y;
	file >> config.texture_size.x >> config.texture_size.y;
	int textures_n;
	file >> textures_n;
	for (int i = 0; i < textures_n; i++)
	{
		string tmp;
		file >> tmp;
		config.texture_paths.push_back(tmp);
	}
	for (int i = 0; i < ANIMATIONS_N; i++)
	{
		string s;
		file >> s;
		config.animation_files.push_back(s);
	}
	Vectorf temp;
	for (int i = 0; i < 4; i++)
	{
		file >> temp.x >> temp.y;
		config.mesh.push_back(temp);
	}
	file >> config.max_hp >> config.height.first >> config.height.second;
	string s;
	file >> s;
	config.sounds.push_back(s);
	file >> s;
	config.sounds.push_back(s);
	file >> s;
	config.sounds.push_back(s);
	return config;
}

void Parser::load_music_config(string path)
{
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::invalid_argument("Music config file not found");
	}
	auto file = util::remove_comments(file_raw);
	while (!file.eof())
	{
		string a, b;
		file >> a >> b;
		if (a == "" || b == "")
		{
			return;
		}
		level_music[a] = b;
	}
}

std::unordered_map<int, string> Parser::load_steps_config(string path)
{
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::invalid_argument("Music config file not found");
	}
	std::unordered_map<int, string> steps_config;
	auto file = util::remove_comments(file_raw);
	while (!file.eof())
	{
		string a, b;
		file >> a >> b;
		if (a == "" || b == "")
		{
			return steps_config;
		}
		steps_config.insert({ static_cast<int>(name_to_surface.at(a)), b });
	}
	return steps_config;
}

std::vector<string> Parser::load_map_sound_config(string path)
{
	std::ifstream file_raw;
	file_raw.open(path);
	if (!file_raw.good())
	{
		throw std::invalid_argument("MAp sounds config file not found");
	}
	std::vector<string> paths;
	auto file = util::remove_comments(file_raw);
	int i = 0;
	while (!file.eof())
	{
		string a, b;
		file >> a >> b;
		if (a == "" || b == "")
		{
			return paths;
		}
		paths.push_back(b);
		map_sounds.insert({ a, i });
		i++;
	}
	return paths;
}

std::unordered_map<string, int> Parser::get_event_map() const
{
	return events;
}

std::pair<std::optional<int>, std::shared_ptr<Moving_platform>>
Parser::parse_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> vert;
		std::unique_ptr<Simple_AI> ai;
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				vert.push_back(parse_vertex(e->GetText(), fliprot));
			}
			else if (n == "vt")
			{
				vert.push_back(parse_textured_vertex(e->GetText()));
			}
			else if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer,
			std::make_shared<Moving_platform>(pos, tex, std::move(vert),
				std::move(ai), surface, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear moving platform" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear platform" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Moving platform error");
}

std::pair<std::optional<int>, std::shared_ptr<Moving_object>>
Parser::parse_moving_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		const sf::Texture* tex;
		Vectorf pos = get_position(element, level_pos);
		string val = get_attribute_by_name("texture", element);
		tex = assets->textures.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		std::unique_ptr<Simple_AI> ai;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_object>(pos, tex,
			height, std::move(ai), fliprot.first, fliprot.second, color));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "moving_object" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Moving object error");
}

std::pair<std::optional<int>, std::shared_ptr<Pendulum>>
Parser::parse_pendulum(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string tex_string = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(tex_string);
		string line_tex_string = get_attribute_by_name("line", element);
		const sf::Texture* line_tex = assets->textures.at(line_tex_string);
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		float line_len = get_and_parse_var<float>("length", element);
		float angle = util::deg_to_rad(get_and_parse_var<float>("angle", element));
		Vectori line_offset = get_and_parse_var<Vectori>("line_offset", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		std::vector<Vectorf> attach_points;
		std::vector<sf::Vertex> vert;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				vert.push_back(parse_vertex(e->GetText(), fliprot));
			}
			else if (n == "vt")
			{
				vert.push_back(parse_textured_vertex(e->GetText()));
			}
			else if (n == "a")
			{
				Vectorf a = parse_var<Vectorf>(e->GetText());
				a *= context.global_scale;
				attach_points.push_back(a);
			}
			else
			{
				std::cout << "Pendulum error" << '\n';
				throw std::invalid_argument("Error in pendulum vertices/attachment points");
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, std::make_shared<Pendulum>(pos, tex, std::move(vert),
			attach_points, angle, line_len, line_tex, line_offset, surface, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "pendulum" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "pendulum" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Pendulum error");
}

std::pair<std::optional<int>, std::shared_ptr<Animated_polygon>> Parser::parse_animated_wall(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		string val = get_attribute_by_name("texture", element);
		const std::vector<const sf::Texture*>* tex = &assets->animations.at(val);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		return std::make_pair(layer,
			std::make_shared<Animated_polygon>(pos, std::move(animation),
				points, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<std::optional<int>, std::shared_ptr<Animated_moving_platform>> Parser::parse_animated_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		string val = get_attribute_by_name("texture", element);
		const std::vector<const sf::Texture*>* tex = &assets->animations.at(val);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		std::unique_ptr<Simple_AI> ai;
		std::vector<sf::Vertex> points;
		sf::Color color = get_and_parse_var<sf::Color>("color", element, sf::Color::White);
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				points.push_back(parse_vertex(e->GetText(), fliprot));
			}
			else if (n == "vt")
			{
				points.push_back(parse_textured_vertex(e->GetText()));
			}
			else if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(layer, std::make_shared<Animated_moving_platform>(pos,
			std::move(animation), points, std::move(ai), surface, color));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<std::optional<int>, std::shared_ptr <Damage_zone>> Parser::parse_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_position(element, level_pos);
		tinyxml2::XMLElement* e = element->FirstChildElement();
		std::vector<std::pair<int, float>> dmg;
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				Vectorf v = parse_var<Vectorf>(e->GetText());
				v *= context.global_scale;
				vert.push_back(v);
			}
			if (n == "d")
			{
				Vectorf d = parse_var<Vectorf>(e->GetText());
				dmg.push_back({ static_cast<int>(d.x), d.y });
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(std::optional<int>(),
			std::make_shared<Damage_zone>(vert, pos, dmg));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "damage zone" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	throw std::runtime_error("damage zone error");
}

std::pair<std::optional<int>, std::shared_ptr<Moving_damage_zone>> Parser::parse_moving_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_position(element, level_pos);
		tinyxml2::XMLElement* e = element->FirstChildElement();
		std::vector<std::pair<int, float>> dmg;
		std::unique_ptr<Simple_AI> ai;
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				Vectorf v = parse_var<Vectorf>(e->GetText());
				v *= context.global_scale;
				vert.push_back(v);
			}
			else if (n == "d")
			{
				Vectorf d = parse_var<Vectorf>(e->GetText());
				dmg.push_back({ static_cast<int>(d.x), d.y });
			}
			else if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(std::optional<int>(),
			std::make_shared<Moving_damage_zone>(vert, pos, dmg, std::move(ai)));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "damage zone" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	throw std::runtime_error("moving damage zone error");
}

std::pair<std::optional<int>, std::shared_ptr<Barrier>> Parser::parse_barrier(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		return std::make_pair(std::optional<int>(),
			std::make_shared<Barrier>(std::move(points), pos, surface));
	}
	catch (const std::exception e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "barrier" << '\n';
	}
	throw std::runtime_error("Barrier error");
}

std::pair<std::optional<int>, std::shared_ptr<Moving_barrier>> Parser::parse_moving_barrier(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_position(element, level_pos);
		int surface = parse_surface(element);
		std::vector<sf::Vertex> vert;
		std::unique_ptr<Simple_AI> ai;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				vert.push_back(parse_vertex(e->GetText(), { 0,0 }));
			}
			else if (n == "move")
			{
				ai = parse_move(e);
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(std::optional<int>(), std::make_shared<Moving_barrier>(std::move(vert), std::move(ai), pos, surface));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "moving barrier" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "moving barrier" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Moving platform error");
}

std::pair<std::optional<int>, std::shared_ptr<Event_zone>> Parser::parse_event_zone(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_position(element, level_pos);
		bool player_only = get_and_parse_var<bool>("player_only", element, false);
		std::vector<int> zone_events;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				Vectorf v = parse_var<Vectorf>(e->GetText());
				v *= context.global_scale;
				vert.push_back(v);
			}
			if (n == "e")
			{
				string name = e->GetText();
				zone_events.push_back(events.at(name));
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(std::optional<int>(),
			std::make_shared<Event_zone>(vert, pos, std::move(zone_events), player_only));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "event zone" << '\n';
		std::cout << "Check vertices" << '\n';
	}
	throw std::runtime_error("event zone error");
}

Map_sound Parser::parse_sound(tinyxml2::XMLElement* element, Vectori level_pos, int id)
{
	try
	{
		Map_sound_info info;
		info.id = id;
		info.repeat = true;
		info.relative = false;
		info.pos = get_position(element, level_pos);
		info.sound = map_sounds.at(get_attribute_by_name("sound", element));
		info.volume = get_and_parse_var<int>("volume", element, 100);
		info.attenuation = get_and_parse_var<float>("attenuation", element, 1.f);
		info.min_distance = get_and_parse_var<float>("min_distance", element, 1.f);
		info.range = get_and_parse_var<float>("range", element, 8.f) * context.global_scale;
		std::vector<Vectorf> vert;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "v")
			{
				Vectorf v = parse_var<Vectorf>(e->GetText());
				v *= context.global_scale;
				vert.push_back(v);
			}
			e = e->NextSiblingElement();
		}
		if (vert.size() > 0)
		{
			return Map_sound(std::move(vert), info);
		}
		else
		{
			return Map_sound(info);
		}
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Check texture" << '\n';
	}
	throw std::runtime_error("Object error");
}

sf::FloatRect Parser::calculate_chunk_bounds(tinyxml2::XMLElement* root,
	std::vector<std::shared_ptr<Map_object>>& objects)
{
	sf::FloatRect bound = sf::FloatRect(INFINITY, INFINITY, 0, 0);
	for (const auto& it : objects)
	{
		sf::FloatRect object_rect = it->get_bounding_rect();
		if (bound.left != INFINITY)
		{
			bound = util::merge_bounds(bound, object_rect);
		}
		else
		{
			bound = object_rect;
		}
	}
	float left = get_and_parse_var<float>("left", root, 0) * context.global_scale;
	bound.left -= left;
	float top = get_and_parse_var<float>("top", root, 0) * context.global_scale;
	bound.top -= top;
	float right = get_and_parse_var<float>("right", root, 0) * context.global_scale;
	bound.width += right + left;
	float bottom = get_and_parse_var<float>("bottom", root, 0) * context.global_scale;
	bound.height += bottom + top;
	return bound;
}


int Parser::parse_surface(tinyxml2::XMLElement* element)
{
	string surface_str = get_attribute_by_name("surface", element);
	if (surface_str == "")
	{
		return 0;
	}
	return name_to_surface.at(surface_str);
}