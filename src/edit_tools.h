#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#include "util.h"
#include "messaging.h"

class Tooltip : public sf::Drawable
{
private:
	sf::RectangleShape rect;
	Vectorf position;
	sf::Text txt;

public:
	Tooltip(Vectorf pos, sf::Font& f, sf::Color c);
	void set_position(Vectorf pos);
	void set_content(string s);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Grid_point : public sf::Drawable
{
private:
	Vectorf position;
	sf::Color color;
	sf::CircleShape point;
	float radius;

public:
	Grid_point(Vectorf point_pos, Vectorf pos_on_level, sf::Color c, float r);
	void set_position(Vectorf pos);
	Vectorf get_position();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Grid : public sf::Drawable, public Message_receiver
{
private:
	sf::VertexBuffer buffer;
	float density;
	float scale;
	const Vectorf level_size;
	const Vectori map_size;
	Vectorf grid_size;
	Vectori lines_n;
	sf::Color color;
	Grid_point cursor_point;
	std::vector<Tooltip> point_tooltips;
	std::vector<Grid_point> points;
	sf::Font* font;
	sf::Color tooltip_color;
	const sf::Color edge_color = sf::Color::Magenta;
	bool add_point_next_update = false;

	void init();
	Vectorf get_closest_node_pos(Vectorf t);//Position on map in pixels
	void append_line(int& n, sf::Vertex* arr, Vectorf begin, Vectorf end, sf::Color line_color);

public:
	Grid(float scale_, float density_, Vectorf level_size_, Vectori map_size_,
		sf::Color grid_color, sf::Color tooltip_color_, sf::Font& font_);
	void set_density(float d);
	void set_color(sf::Color c);
	void update(Vectorf map_mouse_pos);
	void add_point(Vectorf map_mouse_pos);
	void remove_points();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void push_message(Message& msg);
};