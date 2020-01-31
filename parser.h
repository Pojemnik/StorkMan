#pragma once
#include "core.h"
#include "map.h"
#include "assets.h"
#include "tinyxml2.h"
#include <iostream>

Level parse_level(tinyxml2::XMLElement* root, std::shared_ptr<Assets> assets);