 #pragma once
#include "collisions.h"
#include "logic.h"
#include "animations.h"
#include "interfaces.h"

struct Object : public Renderable, public Map_object
{
protected:
	Vectorf pos;
	float height;
	sf::Sprite sprite;
	int flip;
	const float parallax = 1.f;
	const std::array<const sf::Texture*, 3>* tex;

public:
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual sf::FloatRect get_bounding_rect() const;
	Object(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_, float height_,
		int flip_, float angle_, sf::Color color, float parallax_);
};

class Moving_object : public Object, public Graphical_updatable
{
protected:
	std::unique_ptr<Simple_AI> ai;

public:
	Moving_object(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_, float height_,
		std::unique_ptr<Simple_AI> ai_, int flip_, float angle_,
		sf::Color color, float parallax_);
	void update_graphics(float dt);
	void draw(Gbuffer& target, sf::RenderStates states);
	virtual sf::FloatRect get_bounding_rect() const;
	virtual void reset_graphics();
};

class Animated_object : public Animatable, public Object, public Graphical_updatable
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		float height_, int flip_, float angle_, sf::Color color, float parallax_);
	void update_frame();
	void next_frame(float dt);
	void update_graphics(float dt);
	virtual void reset_graphics();
};

class Moving_animated_object : public Animated_object
{
	std::unique_ptr<Simple_AI> ai;

public:
	Moving_animated_object(Vectorf pos_, std::unique_ptr<Animation> animation_,
		float height_, std::unique_ptr<Simple_AI> ai_, int flip_,
		float angle_, sf::Color color, float parallax_);
	void update_graphics(float dt);
	virtual void draw(Gbuffer& target, sf::RenderStates states);
	virtual sf::FloatRect get_bounding_rect() const;
	virtual void reset_graphics();
};