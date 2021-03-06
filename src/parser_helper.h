#pragma once
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "logic.h"
#include "util.h"
#include "collisions.h"

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;
typedef std::string string;

namespace parse
{
	const int DEFAULT_WALL_LAYER = 1;
	const int DEFAULT_PLATFORM_LAYER = 5;
	const int DEFAULT_OBJECT_LAYER = 3;
	const Vectorf fliptab[4] = { {1,1},{-1,1},{1,-1},{-1,-1} };

	std::vector<string> split_string(string s, string d = ",");
	std::string get_attribute_by_name(std::string name, tinyxml2::XMLElement* element);
	std::pair<int, float> parse_flip_rotation(tinyxml2::XMLElement* element);
	int parse_layer(tinyxml2::XMLElement* element, int default_value);
	sf::Vertex parse_vertex(string content, std::pair<int, float> fliprot);
	sf::Vertex parse_textured_vertex(string content);
	std::vector<sf::Vertex> parse_vertices(tinyxml2::XMLElement* element, std::pair<int, float> fliprot);
	void add_vertices(std::vector<sf::Vertex>& vec, const Collision* col);
	Vectorf get_position(tinyxml2::XMLElement* element, const Vectori& level_pos);
	std::string& ltrim(std::string& s);
	std::string& rtrim(std::string& s);
	std::string& trim(std::string& s);

	template <typename T>
	T get_and_parse_var(string name, tinyxml2::XMLElement* element);

	template <typename T>
	T get_and_parse_var(string name, tinyxml2::XMLElement* element, T default_val);

	template <typename T>
	T parse_var(string val);
	template <>
	Vectorf parse_var<Vectorf>(string val);
	template <>
	Vectori parse_var<Vectori>(string val);
	template <>
	bool parse_var<bool>(string val);
	template <>
	sf::Color parse_var<sf::Color>(string val);

	template <typename T>
	T parse_var(string a, string b);
	template <>
	Vectorf parse_var<Vectorf>(string a, string b);
	template <>
	Vectori parse_var<Vectori>(string a, string b);
	template <>
	std::pair<float, float> parse_var<std::pair<float, float>>(string a, string b);

	template <typename T>
	std::unique_ptr<Simple_AI> parse_Simple_AI(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Linear_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Swing_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Swing_rotation_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Accelerated_linear_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Rotation_AI>(tinyxml2::XMLElement* element);
	template <>
	std::unique_ptr<Simple_AI> parse_Simple_AI<Container_AI>(tinyxml2::XMLElement* element);


	std::pair<Vectorf, float> parse_path_node(string content);
	std::tuple<Vectorf, float, float> parse_acc_path_node(string content);
	std::unique_ptr<Simple_AI> parse_move(tinyxml2::XMLElement* element);

	template <typename T>
	inline T get_and_parse_var(string name, tinyxml2::XMLElement* element)
	{
		string s = get_attribute_by_name(name, element);
		if (s == "")
		{
			std::cout << "Critical error: Attribute " << name << " not found!" << std::endl;
			throw std::invalid_argument("Attribute not found!");
		}
		return parse_var<T>(s);
	}

	template<typename T>
	inline T get_and_parse_var(string name, tinyxml2::XMLElement* element, T default_val)
	{
		string s = get_attribute_by_name(name, element);
		if (s == "")
		{
			return default_val;
		}
		return parse_var<T>(s);
	}

	template<typename T>
	inline T parse_var(string val)
	{
		std::stringstream ss(val);
		T var;
		ss >> var;
		return var;
	}

	template <>
	inline sf::Color parse_var<sf::Color>(string val)
	{
		auto vect = split_string(val);
		uint8_t r = (uint8_t)std::stoi(vect[0]);
		uint8_t g = (uint8_t)std::stoi(vect[1]);
		uint8_t b = (uint8_t)std::stoi(vect[2]);
		return sf::Color(r, g, b);
	}

	template <>
	inline Vectori parse_var<Vectori>(string a, string b)
	{
		return Vectori(std::stoi(a), std::stoi(b));
	}

	template <>
	inline Vectorf parse_var<Vectorf>(string a, string b)
	{
		return Vectorf(std::stof(a), std::stof(b));
	}

	template <>
	inline std::pair<float, float> parse_var<std::pair<float, float>>(string a, string b)
	{
		return std::make_pair<float, float>(std::stof(a), std::stof(b));
	}

	template<>
	inline Vectorf parse_var<Vectorf>(string val)
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
	inline Vectori parse_var<Vectori>(string val)
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

	template <>
	inline bool parse_var<bool>(string val)
	{
		return (val == "true" || val == "1");
	}

	template<>
	inline std::unique_ptr<Simple_AI> parse_Simple_AI<Linear_AI>(tinyxml2::XMLElement* element)
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

	template<>
	inline std::unique_ptr<Simple_AI> parse_Simple_AI<Accelerated_linear_AI>(tinyxml2::XMLElement* element)
	{

		std::vector<std::tuple<Vectorf, float, float>> path;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "p")
			{
				path.push_back(parse_acc_path_node(e->GetText()));
			}
			else
			{
				throw(std::invalid_argument(""));
			}
			e = e->NextSiblingElement();
		}
		float time_offset = get_and_parse_var<float>("offset", element, 0.f);
		return std::unique_ptr<Simple_AI>(new Accelerated_linear_AI(path, time_offset));
	}

	template<>
	inline std::unique_ptr<Simple_AI> parse_Simple_AI<Rotation_AI>(tinyxml2::XMLElement* element)
	{
		std::vector<std::pair<float, float>> angles;
		tinyxml2::XMLElement* e = element->FirstChildElement();
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "p")
			{
				auto vect = split_string(e->GetText());
				auto p = parse_var<std::pair<float, float>>(vect[0], vect[1]);
				p.first = util::deg_to_rad(p.first);
				angles.push_back(p);
			}
			else
			{
				throw(std::invalid_argument(""));
			}
			e = e->NextSiblingElement();
		}
		Vectorf pivot = get_and_parse_var<Vectorf>("pivot", element, { 0.f,0.f }) * context.global_scale;
		float time_offset = get_and_parse_var<float>("offset", element, 0.f);
		return std::unique_ptr<Simple_AI>(new Rotation_AI(pivot, angles, time_offset));
	}

	template <>
	inline std::unique_ptr<Simple_AI> parse_Simple_AI<Container_AI>(tinyxml2::XMLElement* element)
	{
		tinyxml2::XMLElement* e = element->FirstChildElement();
		std::vector<std::unique_ptr<Simple_AI>> ais;
		std::unique_ptr<Simple_AI> ai;
		while (e != NULL)
		{
			string n = e->Name();
			if (n == "move")
			{
				ai = parse_move(e);
				ais.push_back(std::move(ai));
			}
			e = e->NextSiblingElement();
		}
		return std::unique_ptr<Simple_AI>(new Container_AI(std::move(ais)));
	}
};