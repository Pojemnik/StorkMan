#pragma once

class Transformable
{
	virtual void move() = 0;
};

class Animatable
{
	virtual void update() = 0;
};

class Drawable
{
	virtual void draw() = 0;
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