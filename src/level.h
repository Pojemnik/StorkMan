#pragma once
#include "worldparts.h"
#include "platforms.h"

class Moving_element : public Updatable, public Renderable
{
	std::shared_ptr<Updatable> updatable;
	std::shared_ptr<Renderable> renderable;
	bool is_drawable;

public:
	int layer;
	bool on_screen;

	sf::FloatRect get_bounding_rect() const;
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Moving_element(std::shared_ptr<Updatable> updatable_, int layer_);
};

class Update_chunk : public Updatable
{
	std::vector<std::shared_ptr<Updatable>> content;

public:
	Update_chunk(std::vector<std::shared_ptr<Updatable>>&& content_);
	void update(float dt);
};

struct Tile : public Updatable
{
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Textured_polygon>> polygons;
	std::vector<std::shared_ptr<Pendulum>> pendulums;
	std::vector<std::shared_ptr<Zone>> zones;

	std::vector<std::shared_ptr<Updatable>> updatables;
	std::vector<std::shared_ptr<Animatable>> animatables;

	std::array<std::vector<std::shared_ptr<Renderable>>, BOTTOM_LAYERS> bottom_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, MIDDLE_LAYERS> middle_layers;
	std::array<std::vector<std::shared_ptr<Renderable>>, TOP_LAYERS> top_layers;

	void update(float dt);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Level
{
	std::vector<std::vector<Tile>> tiles;
	Vectori global_pos;
	Vectori global_size;
	Vectori tiles_n = { 20,20 };
	std::vector<Moving_element> moving;
	Vectori player_tile;

public:
	Level(std::vector<std::vector<Tile>>&& tiles_,
		std::vector<Moving_element>&& moving_, Vectori pos,
		Vectori size);
	void update(float dt, Vectorf player_pos, sf::FloatRect screen_rect);
	void draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const;
};