#pragma once
#include "worldparts.h"
#include "platforms.h"

class Pendulum : public Graphical_updatable, public Physical_updatable, public Renderable, public Collidable, public Map_object
{
	std::vector<Moving_object> lines;
	Moving_platform platform;

public:
	const Collision* const get_collision() const;
	Pendulum(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
		std::vector<sf::Vertex> points_, std::vector<Vectorf> attach_pts,
		float angle_, float line_len_, const std::array<const sf::Texture*, 3>* const line_texture_,
		Vectori line_size, int surface_, sf::Color color);
	void update_physics(float dt);
	void update_graphics(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual sf::FloatRect get_bounding_rect() const;
	virtual Vectorf get_speed() const;
	virtual void reset_graphics();
	virtual void reset_physics();
};