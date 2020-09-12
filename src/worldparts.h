#pragma once
#include "graphics.h"
#include "physics.h"
#include "platforms.h"
#include "logic.h"
#include "animations.h"

struct Object : public Renderable
{
protected:
	Vectorf pos;
	float height;
	sf::Sprite sprite;
	int flip;

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void rescale(float ratio);
	Object() = default;
	Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
		int flip_ = 0, float angle_ = 0);
};

class Moving_object : public Object, public Updatable
{
protected:
	Linear_move move_data;
	float time = 0;
	Vectorf move_pos;

public:
	Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
		Linear_move path, int flip_ = 0, float angle_ = 0);
	Moving_object(const Moving_object&);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void rescale(float ratio);
};

class Animated_object : public Animatable, public Object
{
protected:
	std::unique_ptr<Animation> animation;

public:
	Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		float height_, int flip_ = 0, float angle_ = 0);
	void next_frame(float dt);
};

class Moving_animated_object : public Animated_object, public Updatable
{
private:
	Linear_move move_data;
	float move_time = 0;
	Vectorf move_pos;

public:
	Moving_animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
		float height_, Linear_move path, int flip_ = 0,
		float angle_ = 0);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

struct Wall : public Texturable
{
	Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
		int layer);
};

class Zone
{
public:
	std::vector<Vectorf> vertices;
	Vectorf pos;
	Vectorf center;
	float max_x;
	int id;

	Zone(const std::vector<Vectorf>& vert, Vectorf p);
	Zone(std::vector<Vectorf>& vert, Vectorf p);
	bool contains(Vectorf p);
};

class Damage_zone : public Zone
{
private:
	std::vector<std::pair<int, int>> damage;
	float time = 0;

public:
	std::vector<std::pair<int, int>>::iterator current_damage;
	bool changed_damage = false;

	void update(float dt);
	Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
		std::vector<std::pair<int, int>>& dmg);
	Damage_zone(const Damage_zone& dmgz);
};

inline bool operator==(const Damage_zone& lhs, const Damage_zone& rhs)
{
	return lhs.id == rhs.id;
}

inline bool operator!=(const Damage_zone& lhs, const Damage_zone& rhs)
{
	return !(lhs == rhs);
}