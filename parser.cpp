#include "parser.h"

Parser::Parser(Assets* _assets) : assets(_assets) {};

Vectorf Parser::parse_num_pairf(std::string val)
{
	size_t p = val.find(',');
	if (p == std::string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	float x = std::stof(val.substr(0, p));
	float y = std::stof(val.substr(p + 1));
	return Vectorf(x, y);
}

sf::Color Parser::parse_color(std::string val)
{
	size_t p1, p2;
	p1 = val.find(',');
	if (p1 == std::string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	p2 = val.find(',', p1);
	if (p2 == std::string::npos)
	{
		throw std::invalid_argument("Only one ',' found");
	}
	uint8_t r = std::stoi(val.substr(0, p1));
	uint8_t g = std::stoi(val.substr(p1 + 1, p2));
	uint8_t b = std::stoi(val.substr(p2 + 1));
	return sf::Color(r, g, b);
}

Vectori Parser::parse_num_pairi(std::string val)
{
	size_t p = val.find(',');
	if (p == std::string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	int x = std::stoi(val.substr(0, p));
	int y = std::stoi(val.substr(p + 1));
	return Vectori(x, y);
}

std::string Parser::get_attribute_by_name(std::string name, tinyxml2::XMLElement* element)
{
	tinyxml2::XMLAttribute* att =
		(tinyxml2::XMLAttribute*)(element->FindAttribute(name.c_str()));
	return std::string(att->Value());
}

Level Parser::parse_level(tinyxml2::XMLElement* root)
{
	Level lvl = Level();
	if (root == NULL)
	{
		std::cerr << "B³¹d w pierwszym elemencie pliku!" << std::endl;
		throw std::invalid_argument("NULL pointer");
	}
	else
	{
		std::string root_name = root->Name();
		if (root_name != "level")
		{
			std::cerr << "Brak elementu level" << std::endl;
			throw std::invalid_argument("No level node");
		}
		tinyxml2::XMLElement* element = root->FirstChildElement();
		while (element != NULL)
		{
			std::string name = element->Name();
			if (name == "platform")
			{
				lvl.add_platform(parse_platform(element));
			}
			if (name == "light")
			{
				lvl.add_light_source(parse_light_source(element));
			}
			if (name == "wall")
			{
				lvl.add_wall(parse_wall(element));
			}
			if (name == "object")
			{
				lvl.add_object(parse_object(element));
			}
			element = element->NextSiblingElement();
		}
	}
	return lvl;
}

Light_source Parser::parse_light_source(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_light_source_raw(element);
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "light_source" << std::endl;
		std::cerr << "Nieprawid³owa pozycja lub kolor" << std::endl;
	}
	throw std::runtime_error("Light source error");
}

Light_source Parser::parse_light_source_raw(tinyxml2::XMLElement* element)
{
	const sf::Texture* tex;
	sf::Color color;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	tex = assets->light;
	float intensity = std::stof(get_attribute_by_name("intensity", element));
	color = parse_color(get_attribute_by_name("color", element));
	if (intensity <= 0)
	{
		throw std::invalid_argument("Invalid light intensity");
	}
	return Light_source(pos, tex, color, intensity);
}

Platform Parser::parse_platform(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_platform_raw(element);
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "platform" << std::endl;
		std::cerr << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz platformy" << std::endl;
	}
	catch (const std::out_of_range & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "platform" << std::endl;
		std::cerr << "Prawdopodobnie nieprawid³owa tekstura" << std::endl;
	}
	throw std::runtime_error("Platform error");
}

Platform Parser::parse_platform_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Vertex> points;
	const sf::Texture* tex;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string val = get_attribute_by_name("texture", element);
	tex = assets->textures.at(val);
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			points.push_back(sf::Vertex(v, v));
		}
		else
		{
			std::cerr << "B³¹d w platformie" << std::endl;
			throw std::invalid_argument("Error in platfrom vertices");
		}
		e = e->NextSiblingElement();
	}
	return Platform(pos, tex, points);
}

Wall Parser::parse_wall(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_wall_raw(element);
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "wall" << std::endl;
		std::cerr << "Prawdopodobnie coœ innego ni¿ wierzcho³ek wewn¹trz œciany" << std::endl;
	}
	catch (const std::out_of_range & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "wall" << std::endl;
		std::cerr << "Prawdopodobnie nieprawid³owa tekstura" << std::endl;
	}
	throw std::runtime_error("Wall error");
}

Wall Parser::parse_wall_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Vertex> points;
	const sf::Texture* tex;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string val = get_attribute_by_name("texture", element);
	tex = assets->textures.at(val);
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			points.push_back(sf::Vertex(v, v));
		}
		else
		{
			std::cerr << "B³¹d w œcianie" << std::endl;
			throw std::invalid_argument("Error in wall vertices");
		}
		e = e->NextSiblingElement();
	}
	return Wall(pos, tex, points);
}

Object Parser::parse_object(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_object_raw(element);
	}
	catch (const std::out_of_range & e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl;
		std::cerr << "Element: " << "object" << std::endl;
		std::cerr << "Prawdopodobnie nieprawid³owa tekstura" << std::endl;
	}
	throw std::runtime_error("Object error");
}

Object Parser::parse_object_raw(tinyxml2::XMLElement* element)
{
	const sf::Texture* tex;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string val = get_attribute_by_name("texture", element);
	tex = assets->textures.at(val);
	float height = std::stof(get_attribute_by_name("height", element));
	return Object(pos, tex, height);
}

Map Parser::parse_map(tinyxml2::XMLElement* root)
{
	Vectori map_player_pos = Vectori(-1, -1), map_size = Vectori(-1, -1);
	std::vector<Level> vec;
	if (root == NULL)
	{
		std::cerr << "B³¹d w pierwszym elemencie pliku!" << std::endl;
		throw std::invalid_argument("NULL pointer");
	}
	else
	{
		std::string root_name = root->Name();
		if (root_name != "map")
		{
			std::cerr << "Brak elementu map" << std::endl;
			throw std::invalid_argument("No Map node");
		}
		tinyxml2::XMLAttribute* att;
		map_player_pos = parse_num_pairi(get_attribute_by_name("player_pos", root));
		map_size = parse_num_pairi(get_attribute_by_name("size", root));
		if (map_player_pos == Vectori(-1, -1) || map_size == Vectori(-1, -1))
		{
			std::cerr << "B³¹d w definicji mapy\n";
			throw std::invalid_argument("Invalid map attributes");
		}
		if (map_player_pos.x < 0 || map_player_pos.y < 0)
		{
			throw std::invalid_argument("negative player pos");
		}
		if (map_size.x < 0 || map_size.y < 0)
		{
			throw std::invalid_argument("negative map size");
		}
		tinyxml2::XMLElement* element = root->FirstChildElement();
		while (element != NULL)
		{
			att = (tinyxml2::XMLAttribute*)element->FirstAttribute();
			std::string name = element->Name();
			if (name == "level")
			{
				Vectori pos;
				Vectori size;
				std::string filepath;
				pos = parse_num_pairi(get_attribute_by_name("position", element));
				size = parse_num_pairi(get_attribute_by_name("size", element));
				if (size.x < 0 || size.y < 0)
				{
					throw std::invalid_argument("negative level size");
				}
				if (pos.x < 0 || pos.y < 0)
				{
					throw std::invalid_argument("negative level pos");
				}
				if (size.x + pos.x > map_size.x || size.y + pos.y > map_size.y)
				{
					throw std::invalid_argument("level outside of map size");
				}
				filepath = get_attribute_by_name("filename", element);
				{
					tinyxml2::XMLDocument lvl_root;
					lvl_root.LoadFile(filepath.c_str());
					Level lvl = parse_level(lvl_root.FirstChildElement());
					lvl.global_size = size;
					lvl.global_pos = pos;
					vec.push_back(lvl);
				}
			}
			element = element->NextSiblingElement();
		}
	}
	return Map(map_size, vec, map_player_pos, *assets->bg,
		*assets->layer2, assets->light);
}

void Parser::parse_additional_textures(std::string path)
{
	std::ifstream file;
	std::string p, name;
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