#include "parser.h"

Parser::Parser(Assets* _assets) : assets(_assets) {};

std::pair<Vectori, Vectori> Parser::parse_map_element(tinyxml2::XMLElement* element)
{
	if (element == nullptr)
	{
		std::cout << "B³¹d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer");
	}
	string root_name = element->Name();
	if (root_name != "map")
	{
		std::cout << "Brak elementu map" << '\n';
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
		std::cout << "B³¹d w definicji mapy\n";
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

std::pair<Vectori, string> Parser::parse_level_element(tinyxml2::XMLElement* element, Vectori map_size)
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
	return std::make_pair(pos, filepath);
}

sf::Vertex Parser::parse_vertex(string content, std::pair<int, float> fliprot)
{
	Vectorf v = parse_var<Vectorf>(content);
	v *= context.global_scale;
	Vectorf v2 = util::rotate_vector(v, util::deg_to_rad(fliprot.second));
	v2.x *= fliptab[fliprot.first].x;
	v2.y *= fliptab[fliprot.first].y;
	return sf::Vertex(v, v2);
}

sf::Vertex Parser::parse_textured_vertex(string content)
{
	size_t pos = content.find(",", content.find(",") + 1);
	Vectorf v = parse_var<Vectorf>(content.substr(0, pos));
	v *= context.global_scale;
	Vectorf t = parse_var<Vectorf>(content.substr(pos + 1));
	return sf::Vertex(v, t);
}

std::pair<Vectorf, float> Parser::parse_path_node(string content)
{
	size_t pos2 = content.find(",", content.find(",") + 1);
	Vectorf v = parse_var<Vectorf>(content.substr(0, pos2));
	v *= context.global_scale;
	float time = std::stof(content.substr(pos2 + 1));
	return { v, time };
}

std::vector<sf::Vertex> Parser::parse_vertices(tinyxml2::XMLElement* element, std::pair<int, float> fliprot)
{
	std::vector<sf::Vertex> points;
	while (element != NULL)
	{
		string n = element->Name();
		if (n == "v")
		{
			points.push_back(parse_vertex(element->GetText(), fliprot));
		}
		else if (n == "vt")
		{
			points.push_back(parse_textured_vertex(element->GetText()));
		}
		else
		{
			std::cout << "B³¹d w wierzcho³kach" << '\n';
			throw std::invalid_argument("Error in vertices");
		}
		element = element->NextSiblingElement();
	}
	return points;
}

sf::Color Parser::parse_color(string val)
{
	size_t p1, p2;
	p1 = val.find(',');
	if (p1 == string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	p2 = val.find(',', p1 + 1);
	if (p2 == string::npos)
	{
		throw std::invalid_argument("Only one ',' found");
	}
	uint8_t r = (uint8_t)std::stoi(val.substr(0, p1));
	uint8_t g = (uint8_t)std::stoi(val.substr(p1 + 1, p2));
	uint8_t b = (uint8_t)std::stoi(val.substr(p2 + 1));
	return sf::Color(r, g, b);
}

Surface_type Parser::parse_surface(tinyxml2::XMLElement* element)
{
	string surface_str = get_attribute_by_name("surface", element);
	if (surface_str == "")
	{
		return Surface_type::NONE;
	}
	return name_to_surface.at(surface_str);
}

string Parser::get_attribute_by_name(string name, tinyxml2::XMLElement* element)
{
	tinyxml2::XMLAttribute* att =
		(tinyxml2::XMLAttribute*)(element->FindAttribute(name.c_str()));
	if (att != nullptr)
		return string(att->Value());
	else
		return "";
}

Level Parser::parse_level(tinyxml2::XMLElement* root, Vectori global_pos)
{
	if (root == nullptr)
	{
		std::cout << "B³¹d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer in level file");
	}
	string root_name = root->Name();
	if (root_name != "level")
	{
		std::cout << "Brak elementu level" << '\n';
		throw std::invalid_argument("No level node");
	}
	tinyxml2::XMLElement* element = root->FirstChildElement();
	std::vector<Map_chunk> chunks;
	std::vector<Moving_element> moving;
	while (element != nullptr)
	{
		string name = element->Name();
		if (name == "chunk")
		{
			chunks.push_back(parse_chunk(element, global_pos));
		}
		else if (name == "animated_object")
		{
			auto [layer, ptr] = parse_animated_object(element, global_pos);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "damage_zone")
		{
			auto ptr = parse_damage_zone(element, global_pos);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr));
		}
		else if (name == "pendulum")
		{
			auto [layer, ptr] = parse_pendulum(element, global_pos);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "old_moving_platform")
		{
			auto [layer, ptr] = parse_old_moving_platform(element, global_pos);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "old_moving_object")
		{
			auto [layer, ptr] = parse_old_moving_object(element, global_pos);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		element = element->NextSiblingElement();
	}
	return Level(std::move(chunks), std::move(moving), global_pos);
}

Level Parser::open_and_parse_level(std::pair<Vectori, string> data)
{
	Vectori pos = data.first;
	string filepath = data.second;
	tinyxml2::XMLDocument lvl_root;
	lvl_root.LoadFile(filepath.c_str());
	return parse_level(lvl_root.FirstChildElement(), pos);
}

Map_chunk Parser::parse_chunk(tinyxml2::XMLElement* root, Vectori level_pos)
{
	std::vector<std::shared_ptr<Updatable>> updatables;
	std::vector<std::pair<int, std::shared_ptr<Renderable>>> drawables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	std::vector<std::shared_ptr<Zone>> zones;
	std::vector<std::shared_ptr<Map_object>> map_objects;
	std::vector<sf::Vertex> collision_vertices;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	while (element != nullptr)
	{
		string name = element->Name();
		if (name == "platform")
		{
			auto [layer, ptr] = parse_platform(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
			add_vertices(collision_vertices, collidables.back()->get_collision());
		}
		else if (name == "wall")
		{
			auto [layer, ptr] = parse_wall(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "object")
		{
			auto [layer, ptr] = parse_object(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "animated_object")
		{
			auto [layer, ptr] = parse_animated_object(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "damage_zone")
		{
			auto ptr = parse_damage_zone(element, level_pos);
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			zones.push_back(std::static_pointer_cast<Zone>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "pendulum")
		{
			auto [layer, ptr] = parse_pendulum(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "old_moving_platform")
		{
			auto [layer, ptr] = parse_old_moving_platform(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "moving_platform")
		{
			auto [layer, ptr] = parse_moving_platform(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "old_moving_object")
		{
			auto [layer, ptr] = parse_old_moving_object(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "moving_object")
		{
			auto [layer, ptr] = parse_moving_object(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "barrier")
		{
			auto ptr = parse_barrier(element, level_pos);
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
			add_vertices(collision_vertices, collidables.back()->get_collision());
		}
		else if (name == "animated_wall")
		{
			auto [layer, ptr] = parse_animated_wall(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "animated_moving_platform")
		{
			auto [layer, ptr] = parse_animated_moving_platform(element, level_pos);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
		}
		else if (name == "moving_damage_zone")
		{
			auto ptr = parse_moving_damage_zone(element, level_pos);
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			map_objects.push_back(std::static_pointer_cast<Map_object>(ptr));
			zones.push_back(std::static_pointer_cast<Zone>(ptr));
		}
		element = element->NextSiblingElement();
	}
	sf::FloatRect bound = calculate_chunk_bounds(root, map_objects);
	sf::VertexBuffer collision_buffer(sf::Lines, sf::VertexBuffer::Static);
	collision_buffer.create(collision_vertices.size());
	collision_buffer.update(collision_vertices.data());
	return Map_chunk(std::move(updatables), std::move(drawables),
		std::move(collidables), std::move(zones), bound, std::move(collision_buffer));

}

std::pair<int, std::shared_ptr<Platform>>
Parser::parse_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		Surface_type surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, std::make_shared<Platform>(pos, tex, points, surface));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "platform" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz platformy"
			" lub nieprawid³owa nazwa typu powierzchni"
			<< '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "platform" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Platform error");
}

std::pair<int, std::shared_ptr<Textured_polygon>>
Parser::parse_wall(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_WALL_LAYER);
		return std::make_pair(layer, std::make_shared<Textured_polygon>(pos, tex, points));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "wall" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz œciany" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "wall" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<int, std::shared_ptr<Object>>
Parser::parse_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Object>(pos, tex, height, fliprot.first, fliprot.second));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Object error");
}

std::pair<int, std::shared_ptr<Animated_object>>
Parser::parse_animated_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		const std::vector<sf::Texture>* tex = &assets->animations.at(val);
		float height = get_and_parse_var <float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Animated_object>(pos, std::move(animation),
			height, fliprot.first, fliprot.second));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "object" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa animacja" << '\n';
	}
	throw std::runtime_error("Animated object error");
}

int Parser::parse_layer(tinyxml2::XMLElement* element, int default_value)
{
	int layer = get_and_parse_var<int>("layer", element, default_value);
	if (layer < 0 || layer >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
	{
		throw std::invalid_argument("Invalid layer");
	}
	return layer;
}

std::pair<int, float> Parser::parse_flip_rotation(tinyxml2::XMLElement* element)
{
	float rotationang = get_and_parse_var<float>("rotation", element, 0.f);
	string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_var<Vectori>(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	return std::make_pair(flipint, rotationang);
}

Map Parser::parse_map(tinyxml2::XMLElement* root)
{
	auto map_data = parse_map_element(root);
	Vectori map_size = map_data.first;
	Vectori player_pos = map_data.second;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	Map map(map_size, player_pos, assets->bg);
	std::vector<std::future<Level>> futures;
	while (element != NULL)
	{
		string name = element->Name();
		if (name == "level")
		{
			auto level_data = parse_level_element(element, map_size);
			futures.push_back(std::async(std::launch::async, &Parser::open_and_parse_level, this, level_data));
		}
		else
		{
			throw std::invalid_argument("Incorrect element in map file");
		}
		element = element->NextSiblingElement();
	}
	for (auto& it : futures)
	{
		map.add_level(it.get());
	}
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
	file >> config.tree_file;
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
	return config;
}

std::pair<int, std::shared_ptr<Moving_platform>>
Parser::parse_old_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<std::pair<Vectorf, float>> path;
		float time_offset = get_and_parse_var<float>("offset", element, 0.f);
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
			else if (n == "p")
			{
				path.push_back(parse_path_node(e->GetText()));
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_platform>(pos, tex, std::move(vert),
			std::unique_ptr<Simple_AI>(new Linear_AI(path, time_offset)), Surface_type::NONE));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear platform" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz platformy wahad³a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear platform" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Linear platform error");
}

std::pair<int, std::shared_ptr<Moving_object>>
Parser::parse_old_moving_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		const sf::Texture* tex;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		tex = assets->textures.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<std::pair<Vectorf, float>> path;
		float time_offset = get_and_parse_var<float>("offset", element, 0.f);
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "p")
			{
				path.push_back(parse_path_node(e->GetText()));
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_object>(pos, tex, height,
			std::unique_ptr<Simple_AI>(new Linear_AI(path, time_offset))));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "moving_object" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Moving object error");
}

std::pair<int, std::shared_ptr<Moving_platform>>
Parser::parse_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		Surface_type surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> vert;
		std::unique_ptr<Simple_AI> ai;
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
			else if (n == "linear_move")
			{
				ai = parse_Simple_AI<Linear_AI>(e);
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_platform>(pos, tex, std::move(vert), std::move(ai), surface));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear platform" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz platformy wahad³a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "linear platform" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Linear platform error");

}

std::pair<int, std::shared_ptr<Moving_object>>
Parser::parse_moving_object(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		const sf::Texture* tex;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		tex = assets->textures.at(val);
		float height = get_and_parse_var<float>("height", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::unique_ptr<Simple_AI> ai;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "linear_move")
			{
				ai = parse_Simple_AI<Linear_AI>(e);
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, std::make_shared<Moving_object>(pos, tex, height, std::move(ai)));
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "moving_object" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Moving object error");
}

std::pair<int, std::shared_ptr<Pendulum>>
Parser::parse_pendulum(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		string tex_string = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(tex_string);
		string line_tex_string = get_attribute_by_name("line", element);
		const sf::Texture* line_tex = assets->textures.at(line_tex_string);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		Surface_type surface = parse_surface(element);
		float line_len = get_and_parse_var<float>("length", element);
		float angle = util::deg_to_rad(get_and_parse_var<float>("angle", element));
		Vectori line_offset = get_and_parse_var<Vectori>("line_offset", element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
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
				std::cout << "B³¹d w wahadle" << '\n';
				throw std::invalid_argument("Error in pendulum vertices/attachment points");
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, std::make_shared<Pendulum>(pos, tex, std::move(vert),
			attach_points, angle, line_len, line_tex, line_offset, surface));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "pendulum" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz platformy wahad³a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "pendulum" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Pendulum error");
}

std::pair<int, std::shared_ptr<Animated_polygon>> Parser::parse_animated_wall(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		string val = get_attribute_by_name("texture", element);
		const std::vector<sf::Texture>* tex = &assets->animations.at(val);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		return std::make_pair(layer, std::make_shared<Animated_polygon>(pos, std::move(animation), points));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz œciany" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<int, std::shared_ptr<Animated_moving_platform>> Parser::parse_animated_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		Surface_type surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		string val = get_attribute_by_name("texture", element);
		const std::vector<sf::Texture>* tex = &assets->animations.at(val);
		float frame_time = get_and_parse_var<float>("frame_time", element, 1.f);
		float frame_offset = get_and_parse_var<float>("offset", element, 0.f);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		Static_animation_struct sas(tex, frame_time);
		std::unique_ptr<Animation> animation = std::make_unique<Static_animation>(sas, frame_offset);
		std::unique_ptr<Simple_AI> ai;
		std::vector<sf::Vertex> points;
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
			else if (n == "linear_move")
			{
				ai = parse_Simple_AI<Linear_AI>(e);
			}
			e = e->NextSiblingElement();
		}
		return std::make_pair(layer, std::make_shared<Animated_moving_platform>(pos,
			std::move(animation), points, std::move(ai), surface));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz œciany" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "animated_wall" << '\n';
		std::cout << "Prawdopodobnie nieprawid³owa tekstura" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::shared_ptr <Damage_zone> Parser::parse_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
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
		return std::make_shared<Damage_zone>(vert, pos, dmg);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "damage zone" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz strefy" << '\n';
	}
	throw std::runtime_error("damage zone error");
}

std::shared_ptr<Moving_damage_zone> Parser::parse_moving_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
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
			else if (n == "linear_move")
			{
				ai = parse_Simple_AI<Linear_AI>(e);
			}
			e = e->NextSiblingElement();
		}
		return std::make_shared<Moving_damage_zone>(vert, pos, dmg, std::move(ai));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "damage zone" << '\n';
		std::cout << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz strefy" << '\n';
	}
	throw std::runtime_error("moving damage zone error");
}

std::shared_ptr<Barrier> Parser::parse_barrier(tinyxml2::XMLElement* element, Vectori level_pos)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos += Vectorf(level_pos.x * context.level_size.x,
			level_pos.y * context.level_size.y);
		pos *= context.global_scale;
		Surface_type surface = parse_surface(element);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		return std::make_shared<Barrier>(std::move(points), pos, surface);
	}
	catch (const std::exception e)
	{
		std::cout << "Wyjatek: " << e.what() << '\n';
		std::cout << "Element: " << "barrier" << '\n';
	}
	throw std::runtime_error("Barrier error");
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

void Parser::add_vertices(std::vector<sf::Vertex>& vec, const Collision* col)
{
	for (int i = 1; i < col->mesh.size(); i++)
	{
		vec.emplace_back(col->mesh[i - 1], sf::Color::White);
		vec.emplace_back(col->mesh[i], sf::Color::White);
	}
	vec.emplace_back(col->mesh.back(), sf::Color::White);
	vec.emplace_back(col->mesh.front(), sf::Color::White);
}
