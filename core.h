#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, ATTACK, HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };

const float max_force = 1000.f;
const float max_run_speed = 5.f;

template <typename T> inline int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

class Animation
{
public:
	Vectorf center;
	const std::vector<sf::Texture> content;
	const sf::FloatRect rect_collision;
	std::vector<sf::Texture>::const_iterator begin() const;
	std::vector<sf::Texture>::const_iterator end() const;
	Animation() = default;	//Uwaga domy�lny konstruktor wywo�ywany w Assets::load_aninmation
	Animation(std::vector<sf::Texture>& a, Vectorf c, sf::FloatRect rect_col);
};

class Colidable
{
public:
	sf::FloatRect rect_collision;
	std::vector<Vectorf> mesh_collision;
	Colidable_type type;
	Colidable() = default;
	Colidable(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t);
};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
};

class Physical : public Transformable, public Colidable
{
protected:
	float mass;
	Vectorf speed;
	Vectorf last_speed = { 0,0 };
	Vectorf force;
	Vectori colision_direction = { 0,0 };
public:
	virtual void update() = 0;
	virtual void update_position() = 0;
	virtual void move(Vectorf delta) = 0;
	virtual void apply_force(Vectorf f);
	void uncolide(const Colidable* c);
	Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m);
	Physical() = default;
};

class Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* tex;
	std::vector<sf::Vertex> vertices;
	Vectorf pos;

public:
	Texturable();
	Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Renderable : public sf::Drawable
{
protected:
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;

public:
	Renderable() = default;
	Renderable(Vectorf p, sf::Texture* t, float h);
	void rescale(float gs);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public sf::Drawable
{
protected:
	const Animation* tex;
	Vectorf pos;
	std::vector<sf::Texture>::const_iterator it;
	sf::Sprite sprite;
	int direction = 1;//x sign
	float height; //[m]
	float scale;

public:
	Animatable();
	Animatable(Vectorf p, const Animation* t, float h, float gs);
	void next_frame();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};