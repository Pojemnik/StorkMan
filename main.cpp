#include <iostream>
#include <tinyxml2.h>
#include <fstream>
#include <string>

int main()
{
	tinyxml2::XMLDocument map;
	map.LoadFile("stork_map_example.xml");
	std::string e = map.FirstChildElement("level")->FirstChildElement("platform")->Attribute("position");
	return 0;
}