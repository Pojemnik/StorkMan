#pragma once
#include "game.h"
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"
#include <iostream>

class Parser
{
private:
	Assets* const assets;

	Vectorf parse_num_pairf(std::string val);
	Vectori parse_num_pairi(std::string val);
	std::string get_attribute_by_name(std::string name, tinyxml2::XMLElement* element);
	Level parse_level(tinyxml2::XMLElement* root);
	Platform parse_platform_raw(tinyxml2::XMLElement* element);
	Platform parse_platform(tinyxml2::XMLElement* element);

public:
	Map parse_map(tinyxml2::XMLElement* root);
	Parser(Assets* const _assets);
};
