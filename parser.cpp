#include "parser.h"

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
	tinyxml2::XMLAttribute* att = (tinyxml2::XMLAttribute*)(element->FindAttribute(name.c_str()));
	return std::string(att->Value());
}

Level Parser::parse_level(tinyxml2::XMLElement* root)
{
	Level lvl = Level();
	if (root == NULL)
	{
		std::cerr << "B��d w pierwszym elemencie pliku!" << std::endl;
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
		//tinyxml2::XMLAttribute* att = (tinyxml2::XMLAttribute*)root->FirstAttribute();
		tinyxml2::XMLElement* element = root->FirstChildElement();
		while (element != NULL)
		{
			std::string name = element->Name();
			if (name == "platform")
			{
				lvl.add_platfrom(parse_platform(element));
			}
			if (name == "light")
			{
				lvl.add_light_source(parse_light_source(element));
			}
			element = element->NextSiblingElement();
		}
	}
	return lvl;
}

Light_source Parser::parse_light_source(tinyxml2::XMLElement* element)
{
	Vectorf pos;
	const sf::Texture* tex;
	sf::Color color;
	float intensity;
	pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	tex = assets->light;
	intensity = 0;
	return Light_source(pos, tex, color, intensity);
}

Platform Parser::parse_platform(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_platform_raw(element);
	}
	catch (const std::invalid_argument &e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl << "Element: " << "platform" << std::endl;
		std::cerr << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trze platformy" << std::endl;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Wyjatek: " << e.what() << std::endl << "Element: " << "platform" << std::endl;
		std::cerr << "Prawdopodobnie nieprawid�owa tekstura" << std::endl;
	}
	throw std::runtime_error("Platform error");
}

Platform Parser::parse_platform_raw(tinyxml2::XMLElement* element)
{
	Vectorf pos;
	std::vector<sf::Vertex> points;
	const sf::Texture* tex;
	pos = parse_num_pairf(get_attribute_by_name("position", element));
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
			std::cerr << "B��d w platformie" << std::endl;
			throw std::invalid_argument("Error in platfrom vertices");
		}
		e = e->NextSiblingElement();
	}
	return Platform(pos, tex, points);
}

Map Parser::parse_map(tinyxml2::XMLElement* root)
{
	Vectori map_player_pos = Vectori(-1, -1), map_size = Vectori(-1, -1);
	std::vector<Level> vec;
	if (root == NULL)
	{
		std::cerr << "B��d w pierwszym elemencie pliku!" << std::endl;
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
			std::cerr << "B��d w definicji mapy\n";
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
	return Map(map_size, vec, map_player_pos, *assets->bg, *assets->layer2, assets->light);
}
Parser::Parser(Assets* const _assets) : assets(_assets) {};