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

class Animatable// : Drawable
{
public:
	virtual void update() = 0;
	//Jak bêdzie potrzebne to siê napisze
};

class Drawable : sf::Drawable
{
private:
	sf::Sprite sprite;
	std::shared_ptr<sf::Texture> tex;
	Vectorf pos;
public:
	Drawable();
	Drawable(Vectorf p, std::shared_ptr<sf::Texture> t);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Colidable
{

};

class Object : Drawable, Transformable, Colidable
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