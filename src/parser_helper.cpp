#include "parser_helper.h"


std::vector<string> parse::split_string(string s, string d)
{
	size_t i = 0;
	std::vector<string> vect;
	size_t pos;
	while ((pos = s.find(d, i)) != string::npos)
	{
		string temp = s.substr(i, pos - i);
		temp = trim(temp);
		vect.push_back(temp);
		i = pos + 1;
	}
	string temp = s.substr(i);
	temp = trim(temp);
	vect.push_back(temp);
	return vect;
}

string parse::get_attribute_by_name(string name, tinyxml2::XMLElement* element)
{
	tinyxml2::XMLAttribute* att =
		(tinyxml2::XMLAttribute*)(element->FindAttribute(name.c_str()));
	if (att != nullptr)
		return string(att->Value());
	else
		return "";
}

std::pair<Vectorf, float> parse::parse_path_node(string content)
{
	auto vect = split_string(content);
	if (vect[0] == "-")
	{
		return { Vectorf(NAN,NAN), 0.f };
	}
	Vectorf v = parse_var<Vectorf>(vect[0], vect[1]);
	v *= context.global_scale;
	float time = std::stof(vect[2]);
	return { v, time };
}

std::tuple<Vectorf, float, float> parse::parse_acc_path_node(string content)
{
	auto vect = split_string(content);
	if (vect[0] == "-")
	{
		return { Vectorf(NAN,NAN), 0.f, 0.f };
	}
	Vectorf v = parse_var<Vectorf>(vect[0], vect[1]);
	v *= context.global_scale;
	float time = std::stof(vect[2]);
	float a = std::stof(vect[3]) * context.global_scale;
	return std::tuple<Vectorf, float, float>(v, time, a);
}

std::pair<int, float> parse::parse_flip_rotation(tinyxml2::XMLElement* element)
{
	float rotationang = get_and_parse_var<float>("rotation", element, 0.f);
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

int parse::parse_layer(tinyxml2::XMLElement* element, int default_value)
{
	int layer = get_and_parse_var<int>("layer", element, default_value);
	if (layer < 0 || layer >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
	{
		throw std::invalid_argument("Invalid layer");
	}
	return layer;
}

sf::Vertex parse::parse_vertex(string content, std::pair<int, float> fliprot)
{
	Vectorf v = parse_var<Vectorf>(content);
	v *= context.global_scale;
	Vectorf v2 = util::rotate_vector(v, util::deg_to_rad(fliprot.second));
	v2.x *= fliptab[fliprot.first].x;
	v2.y *= fliptab[fliprot.first].y;
	return sf::Vertex(v, v2);
}

sf::Vertex parse::parse_textured_vertex(string content)
{
	auto vect = split_string(content);
	Vectorf v = parse_var<Vectorf>(vect[0], vect[1]);
	v *= context.global_scale;
	Vectorf t = parse_var<Vectorf>(vect[2], vect[3]);
	return sf::Vertex(v, t);
}

std::vector<sf::Vertex> parse::parse_vertices(tinyxml2::XMLElement* element, std::pair<int, float> fliprot)
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
			std::cout << "B��d w wierzcho�kach" << '\n';
			throw std::invalid_argument("Error in vertices");
		}
		element = element->NextSiblingElement();
	}
	return points;
}

std::unique_ptr<Simple_AI> parse::parse_move(tinyxml2::XMLElement* element)
{
	const static std::unordered_map<string,
		std::function<std::unique_ptr<Simple_AI>(tinyxml2::XMLElement*)>> move_type_map =
	{
		{"linear", parse_Simple_AI<Linear_AI>},
		{"accelerated", parse_Simple_AI<Accelerated_linear_AI>},
		{"rotational", parse_Simple_AI<Rotation_AI>},
		{"container", parse_Simple_AI<Container_AI>}
	};
	return move_type_map.at(get_attribute_by_name("type", element))(element);
}

void parse::add_vertices(std::vector<sf::Vertex>& vec, const Collision* col)
{
	for (int i = 1; i < col->mesh.size(); i++)
	{
		vec.emplace_back(col->mesh[i - 1], sf::Color::White);
		vec.emplace_back(col->mesh[i], sf::Color::White);
	}
	vec.emplace_back(col->mesh.back(), sf::Color::White);
	vec.emplace_back(col->mesh.front(), sf::Color::White);
}

Vectorf parse::get_position(tinyxml2::XMLElement* element, const Vectori& level_pos)
{
	Vectorf pos = get_and_parse_var<Vectorf>("position", element);
	pos += Vectorf(float(level_pos.x * context.level_size.x),
		float(level_pos.y * context.level_size.y));
	pos *= context.global_scale;
	return pos;
}

std::string& parse::ltrim(std::string& s)
{
	auto it = std::find_if(s.begin(), s.end(), 
		[](unsigned char c) {
		return !std::isspace(c);
	});
	s.erase(s.begin(), it);
	return s;
}

std::string& parse::rtrim(std::string& s)
{
	auto it = std::find_if(s.rbegin(), s.rend(),
		[](unsigned char c) {
		return !std::isspace(c);
	});
	s.erase(it.base(), s.end());
	return s;
}

std::string& parse::trim(std::string& s)
{
	return ltrim(rtrim(s));
}