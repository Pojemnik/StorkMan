#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

class Transformable
{
public:
	virtual void move() = 0;
private:
	Vectorf local_position;
};

class Animatable
{
public:
	virtual void update() = 0;
	//Jak bêdzie potrzebne to siê napisze
};

class Drawable
{
public:
	virtual void draw() = 0;
	//Jak bêdzie potrzebne to siê napisze
};

class Colidable
{

};

class Object : Drawable, Transformable, Colidable
{

};

class Entity : Drawable, Transformable, Colidable, Animatable
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