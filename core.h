#pragma once

class Transformable
{

};

class Animatable
{

};

class Drawable
{

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