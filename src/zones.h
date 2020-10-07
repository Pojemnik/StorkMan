#pragma once
#include <unordered_set>
#include "util.h"
#include "entities.h"

class Zone : public Map_object
{
protected:
	std::unordered_set<Entity*, std::hash<Entity*>, Compare_entities> contained;
	Vectorf pos;
	sf::VertexBuffer buffer;

public:
	Collision collision;

	Zone(const std::vector<Vectorf>& vert, Vectorf p);
	virtual void interact(Entity& entity) = 0;
	virtual sf::FloatRect get_bounding_rect() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Damage_zone : public Zone, public Updatable
{
protected:
	std::vector<std::pair<int, float>> damage;
	float time = 0;
	bool changed_damage = false;
	std::vector<std::pair<int, float>>::iterator current_damage;

public:

	Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
		std::vector<std::pair<int, float>>& dmg);
	virtual void update(float dt);
	virtual void interact(Entity& entity);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Moving_damage_zone : public Damage_zone
{
	std::unique_ptr<Simple_AI> ai;
	std::vector<Vectorf> base_mesh;
	sf::FloatRect base_rect;

public:
	Moving_damage_zone(std::vector<Vectorf>& vert, Vectorf p,
		std::vector<std::pair<int, float>>& dmg, std::unique_ptr<Simple_AI>&& ai_);
	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};