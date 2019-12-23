#pragma once

class Object
{

};

class AnimatedObject : Object	//Update method
{

};

class MovingObject : AnimatedObject	//Move method
{

};

class Entity : MovingObject	//Mabye not derive
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