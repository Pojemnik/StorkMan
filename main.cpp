#include <iostream>
#include <tinyxml2.h>
#include <fstream>
#include <string>
#include "map.h"

int main(int argc, char** argv)	//Second argument is a map file for editor
{
	tinyxml2::XMLDocument map;
	map.LoadFile(argv[1]);
	tinyxml2::XMLNode* root = map.FirstChild();
	if (root == NULL)
	{
		std::cout << "B³¹d w pierwszym elemencie pliku!" << std::endl;
	}
	return 0;
}