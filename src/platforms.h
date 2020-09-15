#pragma once
#include "graphics.h"
#include "physics.h"
#include "logic.h"
#include "worldparts.h"

class Barrier : public Collidable
{
private:
	std::vector<Vectorf> vertices;
	Collision collision;

public:
	Barrier(std::vector<Vectorf>&& vertices_, Vectorf pos_);
	const Collision* const get_collision();
};

class Textured_polygon : public Renderable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* texture;

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Textured_polygon(Vectorf pos, const sf::Texture* texture_,
		std::vector<sf::Vertex> points);
};

class Platform : public Textured_polygon, public Collidable
{
	Collision collision;
public:
	const Collision* const get_collision();
	Platform(Vectorf pos_, const sf::Texture* texture_, std::vector<sf::Vertex> points_);
};

class Moving_platform : public Platform, public Updatable
{
	std::unique_ptr<Simple_AI> ai;
public:
	const Collision* const get_collision();
	Moving_platform(Vectorf pos_, const sf::Texture* texture_,
		std::vector<sf::Vertex>&& points_, std::unique_ptr<Simple_AI> ai_);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Pendulum : public Updatable, public Renderable, public Collidable
{
	std::vector<Moving_object> lines;
	Moving_platform platform;
public:
	const Collision* const get_collision();
	Pendulum(Vectorf pos_, const sf::Texture* texture_,
		std::vector<sf::Vertex>&& points_, std::vector<Vectorf> attach_pts,
		float angle_, float line_len_, const sf::Texture* const line_texture_,
		Vectori line_size);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};