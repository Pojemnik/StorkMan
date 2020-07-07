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
	p2 = val.find(',', p1 + 1);
	if (p2 == std::string::npos)
	{
		throw std::invalid_argument("Only one ',' found");
	}
	uint8_t r = (uint8_t)std::stoi(val.substr(0, p1));
	uint8_t g = (uint8_t)std::stoi(val.substr(p1 + 1, p2));
	uint8_t b = (uint8_t)std::stoi(val.substr(p2 + 1));
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
	if (att != nullptr)
		return std::string(att->Value());
	else
		return "";
}

Level Parser::parse_level(tinyxml2::XMLElement* root)
{
	Level lvl = Level();
	if (root == NULL)
	{
		context.console->err << "B��d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer");
	}
	else
	{
		std::string root_name = root->Name();
		if (root_name != "level")
		{
			context.console->err << "Brak elementu level" << '\n';
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
			if (name == "animated_object")
			{
				lvl.add_object(parse_animated_object(element));
			}
			if (name == "pendulum")
			{
				lvl.add_pendulum(parse_pendulum(element));
			}
			if (name == "linear_platform")
			{
				lvl.add_lmp(parse_linear_platform(element));
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
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "light_source" << '\n';
		context.console->err << "Nieprawid�owa pozycja lub kolor" << '\n';
	}
	throw std::runtime_error("Light source error");
}

Light_source Parser::parse_light_source_raw(tinyxml2::XMLElement* element)
{
	const sf::Texture* tex;
	sf::Color color = DEFAULT_LIGHT_COLOR;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	tex = assets->light;
	std::string tmp = get_attribute_by_name("intensity", element);
	float intensity = DEFAULT_LIGHT_INTENSITY;
	if (tmp != "")
		intensity = std::stof(tmp);
	tmp = get_attribute_by_name("color", element);
	if (tmp != "")
		color = parse_color(tmp);
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
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "platform" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "platform" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Platform error");
}

Platform Parser::parse_platform_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Vertex> points;
	std::string visible_str = get_attribute_by_name("visible", element);
	bool visible = true;
	const sf::Texture* tex;
	if (visible_str == "0")
	{
		visible = false;
		tex = new sf::Texture();
	}
	else
	{
		std::string val = get_attribute_by_name("texture", element);
		tex = assets->textures.at(val);
	}
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string layer = get_attribute_by_name("layer", element);
	tinyxml2::XMLElement* e = element->FirstChildElement();
	float rotationang = std::stof(util::pass_or_default(get_attribute_by_name("rotation", element), "0"));
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}

	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			Vectorf v2 = util::rotate_vector(v, rotationang);
			v2.x *= fliptab[flipint].x;
			v2.y *= fliptab[flipint].y;
			points.push_back(sf::Vertex(v, v2));
		}
		else
		{
			context.console->err << "B��d w platformie" << '\n';
			throw std::invalid_argument("Error in platfrom vertices");
		}
		e = e->NextSiblingElement();
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Platform(pos, tex, points, l, visible);
	}
	return Platform(pos, tex, points, DEFAULT_PLATFORM_LAYER, visible);
}

Wall Parser::parse_wall(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_wall_raw(element);
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "wall" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz �ciany" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "wall" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
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
	std::string layer = get_attribute_by_name("layer", element);
	float rotationang = std::stof(util::pass_or_default(get_attribute_by_name("rotation", element), "0"));
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			Vectorf v2 = util::rotate_vector(v, rotationang);
			v2.x *= fliptab[flipint].x;
			v2.y *= fliptab[flipint].y;
			points.push_back(sf::Vertex(v, v2));
		}
		else
		{
			context.console->err << "B��d w �cianie" << '\n';
			throw std::invalid_argument("Error in wall vertices");
		}
		e = e->NextSiblingElement();
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Wall(pos, tex, points, l);
	}
	return Wall(pos, tex, points, DEFAULT_WALL_LAYER);
}

Object Parser::parse_object(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_object_raw(element);
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "object" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
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
	std::string layer = get_attribute_by_name("layer", element);
	std::string rotation = get_attribute_by_name("rotation", element);
	float rotationang = rotation == "" ? 0 : std::stof(rotation);
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Object(pos, tex, height, l, flipint, rotationang);
	}
	return Object(pos, tex, height, DEFAULT_OBJECT_LAYER, flipint, rotationang);
}

Animated_object Parser::parse_animated_object_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Texture>* tex;
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string val = get_attribute_by_name("texture", element);
	tex = &assets->animations.at(val);
	float height = std::stof(get_attribute_by_name("height", element));
	std::string layer = get_attribute_by_name("layer", element);
	std::string rotation = get_attribute_by_name("rotation", element);
	float rotationang = rotation == "" ? 0 : std::stof(rotation);
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	std::string frames_str = get_attribute_by_name("fpf", element);
	int frames = 1;
	std::string frame_delta_str = get_attribute_by_name("delta", element);
	int frame_delta = 0;
	if (frames_str != "")
	{
		frames = std::stoi(frames_str);
	}
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (frame_delta_str != "")
	{
		frame_delta = std::stoi(frame_delta_str);
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Animated_object(pos, tex, height, l, frames, flipint,
			rotationang, frame_delta);
	}
	return Animated_object(pos, tex, height, DEFAULT_OBJECT_LAYER, frames,
		flipint, rotationang, frame_delta);
}

Animated_object Parser::parse_animated_object(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_animated_object_raw(element);
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "object" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa animacja" << '\n';
	}
	throw std::runtime_error("Animated object error");
}

Map Parser::parse_map(tinyxml2::XMLElement* root)
{
	Vectori map_player_pos = Vectori(-1, -1), map_size = Vectori(-1, -1);
	std::vector<Level> vec;
	if (root == NULL)
	{
		context.console->err << "B��d w pierwszym elemencie pliku!" << '\n';
		throw std::invalid_argument("NULL pointer");
	}
	else
	{
		std::string root_name = root->Name();
		if (root_name != "map")
		{
			context.console->err << "Brak elementu map" << '\n';
			throw std::invalid_argument("No Map node");
		}
		tinyxml2::XMLAttribute* att;
		map_player_pos = parse_num_pairi(get_attribute_by_name("player_pos", root));
		map_size = parse_num_pairi(get_attribute_by_name("size", root));
		if (map_player_pos == Vectori(-1, -1) || map_size == Vectori(-1, -1))
		{
			context.console->err << "B��d w definicji mapy\n";
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

void Parser::parse_additional_animations(std::string path)
{
	std::ifstream file;
	std::string p, name;
	int x, y, sx, sy;

	file.open(path);
	if (file.good())
	{
		while (!file.eof())
		{
			file >> p >> name >> x >> y >> sx >> sy;
			assets->load_additional_animation(p, name, Vectori(x, y),
				Vectori(sx, sy));
		}
	}
}

Pendulum Parser::parse_pendulum(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_pendulum_raw(element);
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "pendulum" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy wahad�a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "pendulum" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Pendulum error");
}

Pendulum Parser::parse_pendulum_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Vertex> vert;
	std::vector<Vectorf> attach_points;
	const sf::Texture* tex;
	const sf::Texture* line_tex;
	std::string tex_str = get_attribute_by_name("texture", element);
	tex = assets->textures.at(tex_str);
	std::string line_tex_str = get_attribute_by_name("line", element);
	line_tex = assets->textures.at(line_tex_str);
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	float line_len = std::stof(get_attribute_by_name("length", element));
	float angle = std::stof(get_attribute_by_name("angle", element));
	angle = util::deg_to_rad(angle);
	std::string layer = get_attribute_by_name("layer", element);
	float rotationang = std::stof(util::pass_or_default(get_attribute_by_name("rotation", element), "0"));
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			Vectorf v2 = util::rotate_vector(v, rotationang);
			v2.x *= fliptab[flipint].x;
			v2.y *= fliptab[flipint].y;
			vert.push_back(sf::Vertex(v, v2));
		}
		else if (n == "vt")
		{
			string content = e->GetText();
			size_t pos = content.find(",", content.find(",") + 1);
			Vectorf v = parse_num_pairf(content.substr(0, pos));
			v *= context.global_scale;
			Vectorf t = parse_num_pairf(content.substr(pos + 1));
			vert.push_back(sf::Vertex(v, t));
		}
		else if (n == "a")
		{
			Vectorf a = parse_num_pairf(e->GetText());
			a *= context.global_scale;
			attach_points.push_back(a);
		}
		else
		{
			context.console->err << "B��d w wahadle" << '\n';
			throw std::invalid_argument("Error in pendulum vertices/attachment points");
		}
		e = e->NextSiblingElement();
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Pendulum(tex, line_tex, attach_points, vert, line_len, pos, angle, l);
	}
	return Pendulum(tex, line_tex, attach_points, vert, line_len, pos, angle,
		DEFAULT_PLATFORM_LAYER);
}

Linear_moving_platform Parser::parse_linear_platform(tinyxml2::XMLElement* element)
{
	try
	{
		return parse_linear_platform_raw(element);
	}
	catch (const std::invalid_argument& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "linear platform" << '\n';
		context.console->err << "Prawdopodobnie co� innego ni� wierzcho�ek wewn�trz platformy wahad�a" << '\n';
	}
	catch (const std::out_of_range& e)
	{
		context.console->err << "Wyjatek: " << e.what() << '\n';
		context.console->err << "Element: " << "linear platform" << '\n';
		context.console->err << "Prawdopodobnie nieprawid�owa tekstura" << '\n';
	}
	throw std::runtime_error("Linear platform error");
}

Linear_moving_platform Parser::parse_linear_platform_raw(tinyxml2::XMLElement* element)
{
	std::vector<sf::Vertex> vert;
	const sf::Texture* tex;
	Linear_move path;
	std::string tex_str = get_attribute_by_name("texture", element);
	tex = assets->textures.at(tex_str);
	Vectorf pos = parse_num_pairf(get_attribute_by_name("position", element));
	pos *= context.global_scale;
	std::string layer = get_attribute_by_name("layer", element);
	float rotationang = std::stof(util::pass_or_default(get_attribute_by_name("rotation", element), "0"));
	std::string flip = get_attribute_by_name("flip", element);
	int flipint = 0;
	if (flip != "")
	{
		Vectori flipiv = parse_num_pairi(flip);
		if (flipiv.x < 0)
			flipint = 1;
		if (flipiv.y < 0)
			flipint += 2;
	}
	if (flipint < 0 || flipint >3)
	{
		throw std::invalid_argument("Invalid flip value");
	}
	tinyxml2::XMLElement* e = element->FirstChildElement();
	while (e != NULL)
	{
		std::string n = e->Name();
		if (n == "v")
		{
			Vectorf v = parse_num_pairf(e->GetText());
			v *= context.global_scale;
			Vectorf v2 = util::rotate_vector(v, rotationang);
			v2.x *= fliptab[flipint].x;
			v2.y *= fliptab[flipint].y;
			vert.push_back(sf::Vertex(v, v2));
		}
		else if (n == "vt")
		{
			string content = e->GetText();
			size_t pos = content.find(",", content.find(",") + 1);
			Vectorf v = parse_num_pairf(content.substr(0, pos));
			v *= context.global_scale;
			Vectorf t = parse_num_pairf(content.substr(pos + 1));
			vert.push_back(sf::Vertex(v, t));
		}
		else if (n == "p")
		{
			string content = e->GetText();
			size_t pos2 = content.find(",", content.find(",") + 1);
			Vectorf v = parse_num_pairf(content.substr(0, pos2));
			v += pos;
			v *= context.global_scale;
			float time = std::stof(content.substr(pos2 + 1));
			path.points.push_back({ v, time });
		}
		e = e->NextSiblingElement();
	}
	if (layer != "")
	{
		int l = std::stoi(layer);
		if (l < 0 || l >= BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS)
		{
			throw std::invalid_argument("Invalid layer");
		}
		return Linear_moving_platform(path, tex, pos, vert, l);
	}
	return Linear_moving_platform(path, tex, pos, vert, DEFAULT_PLATFORM_LAYER);
}