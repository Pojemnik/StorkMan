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

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual sf::FloatRect get_bounding_rect() const;
	Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
		int flip_ = 0, float angle_ = 0);
};

class Moving_object : public Object, public Updatable
{
protected:
	std::unique_ptr<Simple_AI> ai;
	Vectorf move_pos;

public:
	Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
		std::unique_ptr<Simple_AI> ai_, int flip_ = 0, float angle_ = 0);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animated_object : public Animatable, public Object
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		float height_, int flip_ = 0, float angle_ = 0);
	void update_frame();
	void next_frame(float dt);
};

class Moving_animated_object : public Animated_object, public Updatable
{
private:
	std::unique_ptr<Simple_AI> ai;

public:
	Moving_animated_object(Vectorf pos_, std::unique_ptr<Animation> animation_,
		float height_, std::unique_ptr<Simple_AI> ai_, int flip_ = 0,
		float angle_ = 0);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Zone : public Map_object
{
	sf::FloatRect bound;

public:
	std::vector<Vectorf> vertices;
	Vectorf pos;
	Vectorf center;
	float max_x;
	int id;

	Zone(const std::vector<Vectorf>& vert, Vectorf p);
	Zone(std::vector<Vectorf>& vert, Vectorf p);
	bool contains(Vectorf p);
	virtual sf::FloatRect get_bounding_rect() const;
};

class Damage_zone : public Zone, public Updatable
{
	std::vector<std::pair<int, int>> damage;
	float time = 0;
	sf::FloatRect bounds;

public:
	std::vector<std::pair<int, int>>::iterator current_damage;
	bool changed_damage = false;

	void update(float dt);
	Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
		std::vector<std::pair<int, int>>& dmg);
	virtual sf::FloatRect get_bounding_rect() const;
};

inline bool operator==(const Damage_zone& lhs, const Damage_zone& rhs)
{
	return lhs.id == rhs.id;
}

inline bool operator!=(const Damage_zone& lhs, const Damage_zone& rhs)
{
	return !(lhs == rhs);
}