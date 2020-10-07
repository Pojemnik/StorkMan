#pragma once
#include <iostream>
#include <sstream>
#include <assert.h>
#include <future>
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"

class Parser
{
	const int DEFAULT_WALL_LAYER = 1;
	const int DEFAULT_PLATFORM_LAYER = 5;
	const int DEFAULT_OBJECT_LAYER = 3;
	const Vectorf fliptab[4] = { {1,1},{-1,1},{1,-1},{-1,-1} };
	const Assets* assets;

	template <typename T>
	T get_and_parse_var(string name, tinyxml2::XMLElement* element);
	template <>
	Vectorf get_and_parse_var<Vectorf>(string name, tinyxml2::XMLElement* element);
	template <>
	Vectori get_and_parse_var<Vectori>(string name, tinyxml2::XMLElement* element);

	template <typename T>
	T get_and_parse_var(string name, tinyxml2::XMLElement* element, T default_val);
	template <>
	Vectorf get_and_parse_var<Vectorf>(string name, tinyxml2::XMLElement* element,
		Vectorf default_val);
	template <>
	Vectori get_and_parse_var<Vectori>(string name, tinyxml2::XMLElement* element,
		Vectori default_val);

	template <typename T>
	T parse_var(string val);
	template <>
	Vectorf parse_var<Vectorf>(string val);
	template <>
	Vectori parse_var<Vectori>(string val);


	template <typename T>
	std::unique_ptr<Simple_AI> parse_Simple_AI(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Linear_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Swing_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Swing_rotation_AI>(tinyxml2::XMLElement* element);


	std::pair<int, float> parse_flip_rotation(tinyxml2::XMLElement* element);
	int parse_layer(tinyxml2::XMLElement* element, int default_value);
	sf::Color parse_color(std::string val);
	std::string get_attribute_by_name(std::string name, tinyxml2::XMLElement* element);
	std::pair<Vectori, Vectori> parse_map_element(tinyxml2::XMLElement* element);
	std::pair<Vectori, string> parse_level_element(tinyxml2::XMLElement* element, Vectori map_size);
	sf::Vertex parse_vertex(string content, std::pair<int, float> fliprot);
	sf::Vertex parse_textured_vertex(string content);
	std::pair<Vectorf, float> parse_path_node(string content);
	std::vector<sf::Vertex> parse_vertices(tinyxml2::XMLElement* element, std::pair<int, float> fliprot);
	Level parse_level(tinyxml2::XMLElement* root, Vectori global_pos);
	Level open_and_parse_level(std::pair<Vectori, string> data);
	Map_chunk parse_chunk(tinyxml2::XMLElement* root, Vectori level_pos);
	std::pair<int, std::shared_ptr<Platform>> parse_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Textured_polygon>> parse_wall(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Object>> parse_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Animated_object>> parse_animated_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Moving_platform>> parse_old_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Moving_object>> parse_old_moving_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Moving_platform>> parse_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Moving_object>> parse_moving_object(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Pendulum>> parse_pendulum(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Animated_polygon>> parse_animated_wall(tinyxml2::XMLElement* element, Vectori level_pos);
	std::pair<int, std::shared_ptr<Animated_moving_platform>> parse_animated_moving_platform(tinyxml2::XMLElement* element, Vectori level_pos);
	std::shared_ptr<Damage_zone> parse_damage_zone(tinyxml2::XMLElement* element, Vectori level_pos);
	std::shared_ptr<Barrier> parse_barrier(tinyxml2::XMLElement* element, Vectori level_pos);
	sf::FloatRect calculate_chunk_bounds(tinyxml2::XMLElement* root,
		std::vector<std::shared_ptr<Map_object>>& objects);
	void add_vertices(std::vector<sf::Vertex>& vec, const Collision* col);

public:
	Map parse_map(tinyxml2::XMLElement* root);
	Entity_config parse_entity_config(string path);
	Parser(Assets* _assets);
};

template <typename T>
T Parser::get_and_parse_var(string name, tinyxml2::XMLElement* element)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		throw std::invalid_argument("Attribute not found!");
	}
	return parse_var<T>(s);
}

template <>
Vectorf Parser::get_and_parse_var<Vectorf>(string name, tinyxml2::XMLElement* element)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		throw std::invalid_argument("Attribute not found!");
	}
	return parse_var<Vectorf>(s);
}

template <>
Vectori Parser::get_and_parse_var<Vectori>(string name, tinyxml2::XMLElement* element)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		throw std::invalid_argument("Attribute not found!");
	}
	return parse_var<Vectori>(s);
}

template<typename T>
inline T Parser::get_and_parse_var(string name, tinyxml2::XMLElement* element, T default_val)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		return default_val;
	}
	return parse_var<T>(s);
}

template<>
inline Vectorf Parser::get_and_parse_var(string name, tinyxml2::XMLElement* element, Vectorf default_val)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		return default_val;
	}
	return parse_var<Vectorf>(s);
}

template<>
inline Vectori Parser::get_and_parse_var(string name, tinyxml2::XMLElement* element, Vectori default_val)
{
	string s = get_attribute_by_name(name, element);
	if (s == "")
	{
		return default_val;
	}
	return parse_var<Vectori>(s);
}

template<typename T>
inline T Parser::parse_var(string val)
{
	std::stringstream ss(val);
	T var;
	ss >> var;
	return var;
}

template<>
inline Vectorf Parser::parse_var<Vectorf>(string val)
{
	size_t p = val.find(',');
	if (p == string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	float x = std::stof(val.substr(0, p));
	float y = std::stof(val.substr(p + 1));
	return Vectorf(x, y);
}

template <>
Vectori Parser::parse_var<Vectori>(string val)
{
	size_t p = val.find(',');
	if (p == string::npos)
	{
		throw std::invalid_argument("No ',' found");
	}
	int x = std::stoi(val.substr(0, p));
	int y = std::stoi(val.substr(p + 1));
	return Vectori(x, y);
}

template<>
inline std::unique_ptr<Simple_AI> Parser::parse_Simple_AI<Linear_AI>(tinyxml2::XMLElement* element)
{

	std::vector<std::pair<Vectorf, float>> path;
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		string n = e->Name();
		if (n == "p")
		{
			path.push_back(parse_path_node(e->GetText()));
		}
		else
		{
			throw(std::invalid_argument(""));
		}
		e = e->NextSiblingElement();
	}
	float time_offset = get_and_parse_var<float>("offset", element, 0.f);
	return std::unique_ptr<Simple_AI>(new Linear_AI(path, time_offset));
}
