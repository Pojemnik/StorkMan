#pragma once
#include "graphics.h"
#include "physics.h"
#include "platforms.h"

struct Wall : public Texturable
{
	Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
		int layer);
};

struct Object : public Renderable
{
	Object(Vectorf p, const sf::Texture* t, float h, int layer);
	Object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
		float ang);
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

class Moving_object : public Object
{
	Linear_move move_data;
	float time = 0;
	Vectorf move_pos;

public:
	Moving_object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
		float ang, Linear_move path);
	Moving_object(const Moving_object&);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animated_object : public Animatable
{
public:
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer, int fr);
	Animated_object(Vectorf p, const std::vector<sf::Texture>* a, float h,
		int layer, int fr, int flip, float ang, int frame_delta);
	void next_frame();

private:
	int frames_diff;
	int frame_counter = 0;
};

class Moving_animated_object : public Animated_object
{
	Linear_move move_data;
	float time = 0;
	Vectorf pos;
public:
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
