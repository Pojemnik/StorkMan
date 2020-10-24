#pragma once
#include "collisions.h"
#include "logic.h"
#include "interfaces.h"

class Barrier : public Collidable, public Map_object
{
protected:
	std::vector<Vectorf> vertices;
	Collision collision;

public:
	Barrier(std::vector<sf::Vertex>&& vertices_, Vectorf pos_, Surface_type surface_);
	virtual const Collision* const get_collision() const;
	virtual sf::FloatRect get_bounding_rect() const;
};

class Moving_barrier : public Barrier, public Updatable
{
	std::unique_ptr<Simple_AI> ai;
	std::vector<Vectorf> base_mesh;
	sf::FloatRect base_rect;
	sf::VertexBuffer vertex;
	Vectorf speed;
	Vectorf pos;

public:

	Moving_barrier(std::vector<sf::Vertex>&& vertices_, std::unique_ptr<Simple_AI> ai_, Vectorf pos_, Surface_type surface_);
	void update(float dt);
	virtual void draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual Vectorf get_speed() const;
};
