#include "edit_tools.h"

void Grid::init()
{
	buffer.setPrimitiveType(sf::PrimitiveType::Lines);
	lines_n = { (int)std::ceil(grid_size.x / scale / density * 2),
		(int)std::ceil(grid_size.y / scale / density * 2) };
	size_t elements = lines_n.x + lines_n.y;
	sf::Vertex* temp;
	temp = new sf::Vertex[elements + 4];
	buffer.create(elements + 4);
	int n = 0;
	double increment = static_cast<double>(density) * scale;
	append_line(n, temp, Vectorf(0, 0), Vectorf(0, grid_size.y), edge_color);
	for (int j = 0; j < map_size.x; j++)
	{
		double offset = j * level_size.x;
		for (double i = increment; i < level_size.x; i += increment)
		{
			append_line(n, temp, Vectorf(i + offset, 0), Vectorf(i + offset, grid_size.y), color);
		}
		append_line(n, temp, Vectorf(level_size.x + offset, 0), Vectorf(level_size.x + offset, grid_size.y), edge_color);
	}
	append_line(n, temp, Vectorf(0, 0), Vectorf(grid_size.x, 0), edge_color);
	for (int j = 0; j < map_size.y; j++)
	{
		double offset = j * level_size.y;
		for (double i = increment; i < level_size.y; i += increment)
		{
			append_line(n, temp, Vectorf(0, i + offset), Vectorf(grid_size.x, i + offset), color);
		}
		append_line(n, temp, Vectorf(0, level_size.y + offset), Vectorf(grid_size.x, level_size.y + offset), edge_color);
	}
	buffer.update(temp);
}

Grid::Grid(float scale_, float density_, Vectorf level_size_, Vectori map_size_,
	sf::Color grid_color, sf::Color tooltip_color_, sf::Font& font_)
	: scale(scale_), density(density_),
	level_size(level_size_* context.global_scale), color(grid_color),
	font(&font_), tooltip_color(tooltip_color_),
	cursor_point({}, sf::Color(grid_color.r, grid_color.g, grid_color.b, grid_color.a + 50), 5.f),
	map_size(map_size_)
{
	grid_size = Vectorf(level_size.x * map_size.x, level_size.y * map_size.y);
	init();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buffer, states);
	target.draw(cursor_point, states);
	for (const auto& it : points)
		target.draw(it, states);
	for (const auto& it : point_tooltips)
		target.draw(it, states);
}

void Grid::push_message(Message& msg)
{
	if (msg.type == Message::Message_type::ADD_GRID_POINT)
	{
		add_point_next_update = true;
	}
	if (msg.type == Message::Message_type::REMOVE_GRID_POINTS)
	{
		remove_points();
	}
}

void Grid::set_density(float d)
{
	density = d;
	init();
}

void Grid::set_color(sf::Color c)
{
	color = c;
	init();
}

void Grid::update(Vectorf map_mouse_pos)
{
	Vectorf pos = get_closest_node_pos(map_mouse_pos);
	cursor_point.set_position(pos);
	if (add_point_next_update)
	{
		add_point(map_mouse_pos);
		add_point_next_update = false;
	}
}

void Grid::add_point(Vectorf map_mouse_pos)
{
	Vectorf pos = get_closest_node_pos(map_mouse_pos);
	const sf::Color c = sf::Color(color.r, color.g, color.b, color.a + 50);
	points.push_back(Grid_point(pos - Vectorf(5.f, 5.f), c, 5.f));
	Vectorf tooltip_pos = Vectorf(pos.x + 5.f, pos.y + 5.f);
	point_tooltips.push_back(Tooltip(tooltip_pos, *font, tooltip_color));
	string s;
	if (points.size() == 1)
	{
		s = std::to_string(pos.x / scale) + " " + std::to_string(pos.y / scale);
	}
	else
	{
		pos -= points[0].get_position();
		pos -= Vectorf(5.f, 5.f);
		s = std::to_string(pos.x / scale) + " " + std::to_string(pos.y / scale);
	}
	point_tooltips.back().set_content(s);
}

void Grid::remove_points()
{
	points.clear();
	point_tooltips.clear();
}

Vectorf Grid::get_closest_node_pos(Vectorf p)
{
	const float sd = scale * density;
	float x = std::max(std::round(p.x / sd) * sd, 0.f);
	float y = std::max(std::round(p.y / sd) * sd, 0.f);
	x = std::min(grid_size.x, x);
	y = std::min(grid_size.y, y);
	if (x == -0.f)
		x = 0;
	if (y == -0.f)
		y = 0;
	return { x,y };
}

void Grid::append_line(int& n, sf::Vertex* arr, Vectorf begin, Vectorf end, sf::Color line_color)
{
	arr[n].color = line_color;
	arr[n++].position = begin;
	arr[n].color = line_color;
	arr[n++].position = end;
}

Tooltip::Tooltip(Vectorf pos, sf::Font& f, sf::Color c)
	: position(pos)
{
	txt.setFont(f);
	txt.setCharacterSize(15);
	txt.setPosition(pos);
	rect.setPosition(pos);
	rect.setFillColor(c);
}

void Tooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
	target.draw(txt, states);
}

void Tooltip::set_position(Vectorf pos)
{
	txt.setPosition(pos);
	rect.setPosition(pos);
}

void Tooltip::set_content(string s)
{
	txt.setString(s);
	rect.setSize({ txt.getGlobalBounds().width,
		txt.getGlobalBounds().height * 2 });
}

Grid_point::Grid_point(Vectorf pos, sf::Color c, float r)
	: position(pos), color(c), radius(r), point(r)
{
	point.setFillColor(c);
	point.setPosition(pos);
}

void Grid_point::set_position(Vectorf pos)
{
	Vectorf r_pos = { pos.x - radius, pos.y - radius };
	position = pos;
	point.setPosition(r_pos);
}

Vectorf Grid_point::get_position()
{
	return position;
}

void Grid_point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(point, states);
}
