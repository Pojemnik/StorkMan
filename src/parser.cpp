#include "parser.h"

Parser::Parser(Assets* _assets) : assets(_assets) {};

std::pair<Vectori, Vectori> Parser::parse_map_element(tinyxml2::XMLElement* element)
{
	if (element == nullptr)
	{
		context.console->err << "B��d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer");
	}
	string root_name = element->Name();
	if (root_name != "map")
	{
		context.console->err << "Brak elementu map" << '\n';
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
		context.console->err << "B��d w definicji mapy\n";
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
}

std::pair<Vectorf, float> Parser::parse_path_node(string content, Vectorf pos)
{
	size_t pos2 = content.find(",", content.find(",") + 1);
	Vectorf v = parse_var<Vectorf>(content.substr(0, pos2));
	v *= context.global_scale;
	v += pos;
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
			context.console->err << "B��d w wierzcho�kach" << '\n';
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
		context.console->err << "B��d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer in level file");
	}
	string root_name = root->Name();
	if (root_name != "level")
	{
		context.console->err << "Brak elementu level" << '\n';
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
			chunks.push_back(parse_chunk(element));
		}
		else if (name == "animated_object")
		{
			auto [layer, object] = parse_animated_object(element);
			std::shared_ptr<Animated_object> ptr(&object);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "damage_zone")
		{
			Damage_zone zone = parse_damage_zone(element);
			std::shared_ptr<Damage_zone> ptr = std::shared_ptr<Damage_zone>(&zone);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr));
		}
		else if (name == "pendulum")
		{
			auto [layer, pendulum] = parse_pendulum(element);
			std::shared_ptr<Pendulum> ptr =
				std::shared_ptr<Pendulum>(&pendulum);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "moving_platform")
		{
			auto [layer, platform] = parse_moving_platform(element);
			std::shared_ptr<Moving_platform> ptr =
				std::shared_ptr<Moving_platform>(&platform);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		else if (name == "moving_object")
		{
			auto [layer, object] = parse_moving_object(element);
			std::shared_ptr<Moving_object> ptr =
				std::shared_ptr<Moving_object>(&object);
			moving.emplace_back(std::static_pointer_cast<Updatable>(ptr), layer);
		}
		element = element->NextSiblingElement();
	}
	return Level(std::move(chunks), std::move(moving), global_pos);
}

Map_chunk Parser::parse_chunk(tinyxml2::XMLElement* root)
{
	std::vector<std::shared_ptr<Updatable>> updatables;
	std::vector<std::pair<int, std::shared_ptr<Renderable>>> drawables;
	std::vector<std::shared_ptr<const Collidable>> collidables;
	tinyxml2::XMLElement* element = root->FirstChildElement();
	while (element != nullptr)
	{
		string name = element->Name();
		if (name == "platform")
		{
			auto [layer, platform] = parse_platform(element);
			std::shared_ptr<Platform> ptr = std::shared_ptr<Platform>(&platform);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
		}
		else if (name == "wall")
		{
			auto [layer, wall] = parse_wall(element);
			std::shared_ptr<Textured_polygon> ptr =
				std::shared_ptr<Textured_polygon>(&wall);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
		}
		else if (name == "object")
		{
			auto [layer, object] = parse_object(element);
			std::shared_ptr<Object> ptr = std::shared_ptr<Object>(&object);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
		}
		else if (name == "animated_object")
		{
			auto [layer, object] = parse_animated_object(element);
			std::shared_ptr<Animated_object> ptr =
				std::shared_ptr<Animated_object>(&object);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
		}
		else if (name == "damage_zone")
		{
			Damage_zone zone = parse_damage_zone(element);
			std::shared_ptr<Damage_zone> ptr = std::shared_ptr<Damage_zone>(&zone);
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
		}
		else if (name == "pendulum")
		{
			auto [layer, pendulum] = parse_pendulum(element);
			std::shared_ptr<Pendulum> ptr =
				std::shared_ptr<Pendulum>(&pendulum);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
		}
		else if (name == "moving_platform")
		{
			auto [layer, platform] = parse_moving_platform(element);
			std::shared_ptr<Moving_platform> ptr =
				std::shared_ptr<Moving_platform>(&platform);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
			collidables.push_back(std::static_pointer_cast<Collidable>(ptr));
		}
		else if (name == "moving_object")
		{
			auto [layer, object] = parse_moving_object(element);
			std::shared_ptr<Moving_object> ptr =
				std::shared_ptr<Moving_object>(&object);
			drawables.emplace_back(layer, std::static_pointer_cast<Renderable>(ptr));
			updatables.push_back(std::static_pointer_cast<Updatable>(ptr));
		}
		element = element->NextSiblingElement();
	}
	sf::FloatRect bound = sf::FloatRect(INFINITY, INFINITY, 0, 0);
	for (const auto& it : updatables)
	{
		std::shared_ptr<Map_object> object = std::dynamic_pointer_cast<Map_object>(it);
		if (object == nullptr)
		{
			continue;
		}
		sf::FloatRect object_rect = object->get_bounding_rect();
		bound.left = std::min(bound.left, object_rect.left);
		bound.top = std::min(bound.top, object_rect.top);
		bound.width = std::max(bound.width, object_rect.width);
		bound.height = std::max(bound.height, object_rect.height);
	}
	string temp = get_attribute_by_name("left", root);
	bound.left += (temp == "") ? 0 : std::stof(temp);
	string temp = get_attribute_by_name("top", root);
	bound.top += (temp == "") ? 0 : std::stof(temp);
	string temp = get_attribute_by_name("width", root);
	bound.width += (temp == "") ? 0 : std::stof(temp);
	string temp = get_attribute_by_name("height", root);
	bound.height += (temp == "") ? 0 : std::stof(temp);
	return Map_chunk(std::move(updatables), std::move(drawables),
		std::move(collidables), bound);

}

std::pair<int, Platform> Parser::parse_platform(tinyxml2::XMLElement* element)
{
	try
	{
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, Platform(pos, tex, points));
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "platform" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "platform" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Platform error");
}

std::pair<int, Textured_polygon> Parser::parse_wall(tinyxml2::XMLElement* element)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<sf::Vertex> points =
			parse_vertices(element->FirstChildElement(), fliprot);
		int layer = parse_layer(element, DEFAULT_WALL_LAYER);
		return std::make_pair(layer, Textured_polygon(pos, tex, points));
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "wall" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz �ciany" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "wall" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}

std::pair<int, Object> Parser::parse_object(tinyxml2::XMLElement* element)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		float height = std::stof(get_attribute_by_name("height", element));
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, Object(pos, tex, height, fliprot.first, fliprot.second));
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "object" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Object error");
}

std::pair<int, Animated_object> Parser::parse_animated_object(tinyxml2::XMLElement* element)
{
	try
	{
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		std::vector<sf::Texture>* tex = &assets->animations.at(val);
		float height = std::stof(get_attribute_by_name("height", element));
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		string frames_str = get_attribute_by_name("frame_time", element);
		float frame_time = 1.f;
		if (frames_str != "")
		{
			frame_time = std::stof(frames_str);
		}
		string frame_delta_str = get_attribute_by_name("offset", element);
		float frame_offset = 0;
		if (frame_delta_str != "")
		{
			frame_offset = std::stof(frame_delta_str);
		}
		Static_animation_struct sas = Static_animation_struct(tex, frame_time);
		std::unique_ptr<Animation> animation = std::unique_ptr<Animation>(&Static_animation(sas, frame_offset));
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, Animated_object(pos, std::move(animation),
			height, fliprot.first, fliprot.second));
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "object" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa animacja" << '\n';
	}
	throw std::runtime_error("Animated object error");
}

int Parser::parse_layer(tinyxml2::XMLElement* element, int default_value)
{
	string layer = get_attribute_by_name("layer", element);
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return l;
	}
	return default_value;
}

std::pair<int, float> Parser::parse_flip_rotation(tinyxml2::XMLElement* element)
{
	string rotation = get_attribute_by_name("rotation", element);
	float rotationang = rotation == "" ? 0 : std::stof(rotation);
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
	Map map(map_size, player_pos);
	while (element != NULL)
	{
		string name = element->Name();
		if (name == "level")
		{
			auto level_data = parse_level_element(element, map_size);
			Vectori pos = level_data.first;
			string filepath = level_data.second;
			{
				tinyxml2::XMLDocument lvl_root;
				lvl_root.LoadFile(filepath.c_str());
				map.add_level(parse_level(lvl_root.FirstChildElement(), pos));
			}
		}
		else
		{
			throw std::invalid_argument("Incorrect element in map file");
		}
		element = element->NextSiblingElement();
	}
	return map;
}

void Parser::parse_additional_textures(string path)
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
			assets->load_additional_texture(p, name, repeat);
		}
	}
}

void Parser::parse_additional_animations(string path)
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
			assets->load_additional_animation(p, name, Vectori(x, y),
				Vectori(sx, sy));
		}
	}
}

std::pair<int, Moving_platform> Parser::parse_moving_platform(tinyxml2::XMLElement* element)
{
	try
	{
		std::vector<sf::Vertex> vert;
		string val = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(val);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<std::pair<Vectorf, float>> path;
		string time_offset_str = get_attribute_by_name("offset", element);
		float time_offset = 0;
		if (time_offset_str != "")
		{
			time_offset = std::stof(time_offset_str);
		}
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
				path.push_back(parse_path_node(e->GetText(), pos));
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, Moving_platform(pos, tex, std::move(vert),
			std::make_unique<Simple_AI>(Linear_AI(path, time_offset))));
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "linear platform" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy wahad�a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "linear platform" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Linear platform error");
}

std::pair<int, Moving_object> Parser::parse_moving_object(tinyxml2::XMLElement* element)
{
	try
	{
		const sf::Texture* tex;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		string val = get_attribute_by_name("texture", element);
		tex = assets->textures.at(val);
		float height = std::stof(get_attribute_by_name("height", element));
		std::pair<int, float> fliprot = parse_flip_rotation(element);
		std::vector<std::pair<Vectorf, float>> path;
		string time_offset_str = get_attribute_by_name("offset", element);
		float time_offset = 0;
		if (time_offset_str != "")
		{
			time_offset = std::stof(time_offset_str);
		}
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "p")
			{
				path.push_back(parse_path_node(e->GetText(), pos));
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_OBJECT_LAYER);
		return std::make_pair(layer, Moving_object(pos, tex, height, std::make_unique<Simple_AI>(Linear_AI(path, time_offset))));
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "moving_object" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Moving object error");
}

std::pair<int, Pendulum> Parser::parse_pendulum(tinyxml2::XMLElement* element)
{
	try
	{
		string tex_string = get_attribute_by_name("texture", element);
		const sf::Texture* tex = assets->textures.at(tex_string);
		string line_tex_string = get_attribute_by_name("line", element);
		const sf::Texture* line_tex = assets->textures.at(line_tex_string);
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		float line_len = std::stof(get_attribute_by_name("length", element));
		float angle = util::deg_to_rad(std::stof(get_attribute_by_name("angle", element)));
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
				context.console->err << "B��d w wahadle" << '\n';
				throw std::invalid_argument("Error in pendulum vertices/attachment points");
			}
			e = e->NextSiblingElement();
		}
		int layer = parse_layer(element, DEFAULT_PLATFORM_LAYER);
		return std::make_pair(layer, Pendulum(pos, tex, std::move(vert),
			attach_points, angle, line_len, line_tex, line_offset));
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "pendulum" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy wahad�a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "pendulum" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Pendulum error");
}

Damage_zone Parser::parse_damage_zone(tinyxml2::XMLElement* element)
{
	try
	{
		std::vector<Vectorf> vert;
		Vectorf pos = get_and_parse_var<Vectorf>("position", element);
		pos *= context.global_scale;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		std::vector<std::pair<int, int>> dmg;
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
				Vectori d = parse_var<Vectori>(e->GetText());
				dmg.push_back({ d.x, d.y });
			}
			e = e->NextSiblingElement();
		}
		return Damage_zone(vert, pos, dmg);
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "wall" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz strefy" << '\n';
	}
	throw std::runtime_error("Textured_polygon error");
}