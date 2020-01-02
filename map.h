#pragma once
#include "core.h"
#include <vector>

class Platform : Object
{

};

class Level
{
	std::vector<Object> objects;
	std::vector<Transformable> movables;
	std::vector<Animatable> animatables;	//Great name
};