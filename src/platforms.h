#pragma once
#include "collisions.h"
#include "logic.h"
#include "interfaces.h"

class Barrier : public Collidable, public Map_object
{
private:
	std::vector<Vectorf> vertices;
	Collision collision;

public:
	Barrier(std::vector<Vectorf>&& vertices_, Vectorf pos_);
	const Collision* const get_collision();
	virtual sf::FloatRect get_bounding_rect() const;
};

class Textured_polygon : public Renderable
{
protected:
	sf::VertexBuffer shape;
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
	std::unique_ptr<Simple_AI> ai;
	std::vector<Vectorf> base_mesh;
	sf::FloatRect base_rect;

public:
	const Collision* const get_collision() const;
	Moving_platform(Vectorf pos_, const sf::Texture* texture_,
		std::vector<sf::Vertex>&& points_, std::unique_ptr<Simple_AI> ai_);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
