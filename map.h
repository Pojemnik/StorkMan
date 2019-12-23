#pragma once
#include "core.h"
#include <vector>

class Platform : Object
{

};

class MovingPlatform : MovingObject
{

};

class Level
{
	std::vector<Object> objects;
	std::vector<MovingObject> movables;
	std::vector<AnimatedObject> animatables;	//Great name
};