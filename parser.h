#pragma once
#include "game.h"
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"
#include <iostream>
//#include <SFML/Graphics.hpp>

Vectorf parse_num_pairf(std::string);
Vectori parse_num_pairi(std::string);
std::string get_attribute_by_name(std::string,tinyxml2::XMLElement);
Level parse_level(tinyxml2::XMLElement* root, Assets* assets);
Map parse_map(tinyxml2::XMLElement* root, Assets* assets);