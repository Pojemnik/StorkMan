#include <iostream>
#include <tinyxml2.h>
#include <fstream>

int main()
{
	//std::fstream file;
	//file.open("C:\PulpitS\WorkingDir\StorkMan\stork_map_example.xml");
	tinyxml2::XMLDocument map;
	map.LoadFile("stork_map_example.xml");
	map.FirstChildElement("level")->FirstChildElement("plaform")->FindAttribute("position");
	return 0;
}