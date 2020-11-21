#pragma once
#include <iostream>
#include <sstream>
#include <assert.h>
#include <future>
#include <optional>
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"
#include "parser_helper.h"
#include "barriers.h"
#include "zones.h"
#include "map_sounds.h"

using namespace parse;

class Parser
{
	const Assets* assets;
	std::unordered_map<string, string> level_music;
	std::unordered_map<string, int> map_sounds;

	std::pair<Vectori, Vectori> parse_map_element(tinyxml2::XMLElement* element);
	std::tuple<Vectori, string, string> parse_level_element(tinyxml2::XMLElement* element, Vectori map_size);
	Level parse_level(tinyxml2::XMLElement* root, Vectori global_pos, int code);
	std::unique_ptr<Level> open_and_parse_level(Vectori pos, string path, int code);
	Map_chunk parse_chunk(tinyxml2::XMLElement* root, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Platform>>
		parse_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Textured_polygon>>
		parse_wall(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Object>>
		parse_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Animated_object>>
		parse_animated_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_animated_object>>
		parse_animated_moving_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_platform>>
		parse_old_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_object>>
		parse_old_moving_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_platform>>
		parse_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_object>>
		parse_moving_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Pendulum>>
		parse_pendulum(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Animated_polygon>>
		parse_animated_wall(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Animated_moving_platform>>
		parse_animated_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Damage_zone>> parse_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_damage_zone>> parse_moving_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Barrier>> parse_barrier(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<std::optional<int>, std::shared_ptr<Moving_barrier>> parse_moving_barrier(tinyxml2::XMLElement* element, Vectori level_pos);
	sf::FloatRect calculate_chunk_bounds(tinyxml2::XMLElement* root,
		std::vector<std::shared_ptr<Map_object>>& objects);
	const std::unordered_map<
		std::string, std::function<std::pair<std::optional<int>, std::shared_ptr<Map_object>>(tinyxml2::XMLElement*, Vectori)>>
		element_name_to_function_returning_pair =
	{
		{"platform", std::bind(&Parser::parse_platform, this, std::placeholders::_1, std::placeholders::_2)},
		{"wall", std::bind(&Parser::parse_wall, this, std::placeholders::_1, std::placeholders::_2)},
		{"object", std::bind(&Parser::parse_object, this, std::placeholders::_1, std::placeholders::_2)},
		{"animated_object", std::bind(&Parser::parse_animated_object, this, std::placeholders::_1, std::placeholders::_2)},
		{"animated_moving_object", std::bind(&Parser::parse_animated_moving_object, this, std::placeholders::_1, std::placeholders::_2)},
		{"old_moving_platform", std::bind(&Parser::parse_old_moving_platform, this, std::placeholders::_1, std::placeholders::_2)},
		{"old_moving_object", std::bind(&Parser::parse_old_moving_object, this, std::placeholders::_1, std::placeholders::_2)},
		{"moving_platform", std::bind(&Parser::parse_moving_platform, this, std::placeholders::_1, std::placeholders::_2)},
		{"moving_object", std::bind(&Parser::parse_moving_object, this, std::placeholders::_1, std::placeholders::_2)},
		{"pendulum", std::bind(&Parser::parse_pendulum, this, std::placeholders::_1, std::placeholders::_2)},
		{"animated_wall", std::bind(&Parser::parse_animated_wall, this, std::placeholders::_1, std::placeholders::_2)},
		{"animated_moving_platform", std::bind(&Parser::parse_animated_moving_platform, this, std::placeholders::_1, std::placeholders::_2)},
		{"damage_zone", std::bind(&Parser::parse_damage_zone, this, std::placeholders::_1, std::placeholders::_2)},
		{"moving_damage_zone", std::bind(&Parser::parse_moving_damage_zone, this, std::placeholders::_1, std::placeholders::_2)},
		{"barrier", std::bind(&Parser::parse_barrier, this, std::placeholders::_1, std::placeholders::_2)},
		{"moving_barrier", std::bind(&Parser::parse_moving_barrier, this, std::placeholders::_1, std::placeholders::_2)}
	};

public:
	std::unordered_map<string, int> level_names;
	std::vector<string> music_paths;
	Map parse_map(tinyxml2::XMLElement* root);
	Entity_config parse_entity_config(string path);
	void load_music_config(string path);
	std::unordered_map<int, string> load_steps_config(string path);
	std::vector<string> load_map_sound_config(string path);
	Parser(Assets* _assets);
};
