#pragma once
#include "entities.h"
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
	const Vectorf fliptab[4] = { {1,1},{-1,1},{1,-1},{-1,-1} };
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
	Moving_object parse_moving_object_raw(tinyxml2::XMLElement* element);
	Moving_object parse_moving_object(tinyxml2::XMLElement* element);
	Animated_object parse_animated_object_raw(tinyxml2::XMLElement* element);
	Animated_object parse_animated_object(tinyxml2::XMLElement* element);
	Pendulum parse_pendulum(tinyxml2::XMLElement* element);
	Pendulum parse_pendulum_raw(tinyxml2::XMLElement* element);
	Linear_moving_platform parse_linear_platform(tinyxml2::XMLElement* element);
	Linear_moving_platform parse_linear_platform_raw(tinyxml2::XMLElement* element);
	Damage_zone parse_damage_zone(tinyxml2::XMLElement* element);
	Damage_zone parse_damage_zone_raw(tinyxml2::XMLElement* element);
	std::pair<int, float> parse_flip_rotation(tinyxml2::XMLElement* element);
	int parse_layer(tinyxml2::XMLElement* element, int default_value);

public:
	Map parse_map(tinyxml2::XMLElement* root);
	void parse_additional_textures(std::string path);
	void parse_additional_animations(std::string path);
	Parser(Assets* _assets);
};