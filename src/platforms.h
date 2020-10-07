#pragma once
#include "collisions.h"
#include "logic.h"
#include "interfaces.h"
#include "animations.h"

class Barrier : public Collidable, public Map_object
{
private:
	std::vector<Vectorf> vertices;
	Collision collision;

public:
	Barrier(std::vector<sf::Vertex>&& vertices_, Vectorf pos_);
	const Collision* const get_collision() const;
	virtual sf::FloatRect get_bounding_rect() const;
};

class Textured_polygon : public Renderable, public Map_object
{
protected:
	sf::VertexBuffer polygon;
	const sf::Texture* texture;
	sf::FloatRect bound;

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Textured_polygon(Vectorf pos, const sf::Texture* texture_,
		std::vector<sf::Vertex> points);
	virtual sf::FloatRect get_bounding_rect() const;
};

class Platform : public Textured_polygon, public Collidable
{
protected:
	Collision collision;

public:
	const Collision* const get_collision() const;
	Platform(Vectorf pos_, const sf::Texture* texture_, std::vector<sf::Vertex> points_);
	virtual sf::FloatRect get_bounding_rect() const;
};

class Moving_platform : public Platform, public Updatable
{
protected:
	std::unique_ptr<Simple_AI> ai;
	std::vector<Vectorf> base_mesh;
	sf::FloatRect base_rect;
	sf::VertexBuffer vertex;

public:
	const Collision* const get_collision() const;
	Moving_platform(Vectorf pos_, const sf::Texture* texture_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animated_polygon : public Textured_polygon, public Animatable, public Updatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_polygon(Vectorf pos, std::unique_ptr<Animation>&& animation_,
		std::vector<sf::Vertex> points);
	virtual void update_frame();
	virtual void next_frame(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual sf::FloatRect get_bounding_rect() const;
	virtual void update(float dt);
};

class Animated_moving_platform : public Moving_platform, public Animatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_moving_platform(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_);
	void update(float dt);
	virtual void update_frame();
	virtual void next_frame(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const;
};
