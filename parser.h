#pragma once
#include "game.h"
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"

class Parser
{
private:
	const int DEFAULT_WALL_LAYER = 1;
	const int DEFAULT_PLATFORM_LAYER = 5;
	const int DEFAULT_OBJECT_LAYER = 3;
	const float DEFAULT_LIGHT_INTENSITY = 1;
	const sf::Color DEFAULT_LIGHT_COLOR = sf::Color::White;
	Assets* assets;

	Vectorf parse_num_pairf(std::string val);
	sf::Color parse_color(std::string val);
	Vectori parse_num_pairi(std::string val);
	std::string get_attribute_by_name(std::string name, tinyxml2::XMLElement* element);
	Level parse_level(tinyxml2::XMLElement* root);
	Platform parse_platform_raw(tinyxml2::XMLElement* element);
	Platform parse_platform(tinyxml2::XMLElement* element);
	Light_source parse_light_source_raw(tinyxml2::XMLElement* element);
	Light_source parse_light_source(tinyxml2::XMLElement* element);
	Wall parse_wall_raw(tinyxml2::XMLElement* element);
	Wall parse_wall(tinyxml2::XMLElement* element);
	Object parse_object_raw(tinyxml2::XMLElement* element);
	Object parse_object(tinyxml2::XMLElement* element);

public:
	Map parse_map(tinyxml2::XMLElement* root);
	void parse_additional_textures(std::string path);
	Parser(Assets* _assets);
};
