#pragma once
#include "collisions.h"
#include "logic.h"
#include "interfaces.h"
#include "animations.h"

class Textured_polygon : public Renderable, public Map_object
{
protected:
	sf::VertexBuffer polygon;
	const std::array<const sf::Texture*, 3>* texture;
	sf::FloatRect bound;

public:
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	Textured_polygon(Vectorf pos, const std::array<const sf::Texture*, 3>* texture_,
		std::vector<sf::Vertex> points, sf::Color color);
	virtual sf::FloatRect get_bounding_rect() const;
};

class Platform : public Textured_polygon, public Collidable
{
protected:
	Collision collision;

public:
	const Collision* const get_collision() const;
	Platform(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
		std::vector<sf::Vertex> points_, int surface_, bool one_sided,
		sf::Color color);
	virtual sf::FloatRect get_bounding_rect() const;
};

class Moving_platform : public Platform, public Physical_updatable
{
protected:
	std::unique_ptr<Simple_AI> ai;
	std::vector<Vectorf> base_mesh;
	sf::FloatRect base_rect;
	sf::VertexBuffer vertex;
	Vectorf speed;
	Vectorf pos;

public:
	virtual const Collision* const get_collision() const;
	Moving_platform(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
		int surface_, sf::Color color);
	void update_physics(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual Vectorf get_speed() const;
	virtual void reset_physics();
};

class Animated_polygon : public Textured_polygon, public Animatable, public Graphical_updatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_polygon(Vectorf pos, std::unique_ptr<Animation>&& animation_,
		std::vector<sf::Vertex> points, sf::Color color);
	virtual void update_frame();
	virtual void next_frame(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual sf::FloatRect get_bounding_rect() const;
	virtual void update_graphics(float dt);
	virtual void reset_graphics();
};

class Animated_moving_platform : public Moving_platform, public Animatable, public Graphical_updatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_moving_platform(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
		int surface_, sf::Color color);
	void update_physics(float dt);
	void update_graphics(float dt);
	virtual void update_frame();
	virtual void next_frame(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void reset_graphics();
};

class Moving_polygon : public Textured_polygon, public Graphical_updatable
{
protected:
	std::unique_ptr<Simple_AI> ai;

public:
	Moving_polygon(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
		sf::Color color);
	void update_graphics(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual void reset_graphics();
};

class Animated_moving_polygon : public Moving_polygon, public Animatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_moving_polygon(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
		sf::Color color);
	void update_graphics(float dt);
	virtual void update_frame();
	virtual void next_frame(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual void reset_graphics();
};