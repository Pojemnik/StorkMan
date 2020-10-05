#pragma once
#include "worldparts.h"
#include "platforms.h"

class Pendulum : public Updatable, public Renderable, public Collidable, public Map_object
{
	std::vector<Moving_object> lines;
	Moving_platform platform;
public:
	const Collision* const get_collision() const;
	Pendulum(Vectorf pos_, const sf::Texture* texture_,
		std::vector<sf::Vertex> points_, std::vector<Vectorf> attach_pts,
		float angle_, float line_len_, const sf::Texture* const line_texture_,
		Vectori line_size);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual sf::FloatRect get_bounding_rect() const;
};