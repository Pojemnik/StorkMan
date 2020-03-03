#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

enum Entity_status{IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, ATTACK, HIT};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
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
	sf::Sprite sprite;
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]

public:
	Renderable() = default;
	Renderable(Vectorf p, sf::Texture* t, float h);
	void rescale(float gs);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public sf::Drawable
{
protected:
	const std::vector<sf::Texture>* tex;
	Vectorf pos;
	std::vector<sf::Texture>::const_iterator it;
	sf::Sprite sprite;
	int direction = 1;//x sign
	float height; //[m]

public:
	Animatable();
	Animatable(Vectorf p, const std::vector<sf::Texture>* t, float h, float gs);
	void next_frame();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Colidable
{
	std::vector<sf::Vector2f> vert;
};


class Platform : public Texturable
{
public:
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
};


class Object : Renderable, Transformable, Colidable
{
public:
	Object(Vectorf pos, sf::Texture* t);
};

class Entity : public Animatable, public Colidable, public Transformable
{
private:
	std::vector<const std::vector<sf::Texture>*> animations;
public:
	Entity_status status;
	void set_animation(const std::vector<sf::Texture>* t);
	Entity(Vectorf p, std::vector<const std::vector<sf::Texture>*> t, float h, float gs);
	void move(Vectorf delta);
	void next_frame();
};

class Player : public Entity
{
public:
	Player(Vectorf p, std::vector<const std::vector<sf::Texture>* > t, float h, float gs);
};

class Enemy : Entity
{

};

class NPC : Entity
{

};