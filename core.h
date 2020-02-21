#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

enum Entity_status{IDLE = 0, MOVE, ATTACK, HIT};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
};

class Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	std::shared_ptr<sf::Texture> tex;
	std::vector<sf::Vertex> vertices;
	Vectorf pos;

public:
	Texturable();
	Texturable(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<sf::Vertex> points);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Renderable : public sf::Drawable
{
protected:
	sf::Sprite sprite;
	std::shared_ptr<sf::Texture> tex;
	Vectorf pos;
	float height; //[m]

public:
	Renderable() = default;
	Renderable(Vectorf p, std::shared_ptr<sf::Texture> t, float h);
	void rescale(float gs);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public Renderable
{
protected:
	std::shared_ptr<std::vector<sf::Texture>> tex;
	Vectorf pos;
	sf::Sprite sprite;
	std::vector<sf::Texture>::iterator it;
	int direction = 1;//x sign
	float height; //[m]

public:
	Animatable();
	Animatable(Vectorf p, std::shared_ptr<std::vector<sf::Texture>> t, float h, float gs);
	void next_frame();
	void set_animation(std::shared_ptr<std::vector<sf::Texture>> t);
};

class Colidable
{
	std::vector<sf::Vector2f> vert;
};


class Platform : public Texturable
{
public:
	Platform(Vectorf p, std::shared_ptr<sf::Texture> t, std::vector<sf::Vertex> points);
};


class Object : Renderable, Transformable, Colidable
{
public:
	Object(Vectorf pos, std::shared_ptr<sf::Texture> t);
};

class Entity : public Animatable, public Colidable, public Transformable
{
private:
	std::vector<std::shared_ptr<std::vector<sf::Texture>>> animations;
public:
	Entity_status status;
	Entity(Vectorf p, std::vector<std::shared_ptr<std::vector<sf::Texture>>> t, float h, float gs);
	void move(Vectorf delta);
	void next_frame();
};

class Player : public Entity
{
public:
	Player(Vectorf p, std::vector<std::shared_ptr<std::vector<sf::Texture>>> t, float h, float gs);
};

class Enemy : Entity
{

};

class NPC : Entity
{

};