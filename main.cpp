#include <iostream>
#include <tinyxml2.h>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
	tinyxml2::XMLDocument map;
	map.LoadFile("stork_map_example.xml");
	return 0;
}