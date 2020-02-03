#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

class Transformable
{
public:
	virtual void move() = 0;
private:
	Vectorf local_position;
};

class Animatable// : Drawable
{
public:
	virtual void update() = 0;
	//Jak bêdzie potrzebne to siê napisze
};

class Texturable : public sf::Drawable
{
private:
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
private:
	sf::Sprite sprite;
	std::shared_ptr<sf::Texture> tex;
	Vectorf pos;
public:
	Renderable();
	Renderable(Vectorf p, std::shared_ptr<sf::Texture> t);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Colidable
{

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

class Entity : Transformable, Colidable, Animatable
{

};

class Player : Entity
{

};

class Enemy : Entity
{

};

class NPC : Entity
{

};