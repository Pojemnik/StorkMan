#include "parser.h"

Level parse_level(tinyxml2::XMLElement* root, std::shared_ptr<Assets> assets)
{
	Level lvl = Level();
	if (root == NULL)
	{
		std::cout << "B³¹d w pierwszym elemencie pliku!" << std::endl;
		throw std::invalid_argument("NULL pointer");
	}
	else
	{
		std::string root_name = root->Name();
		if (root_name != "level")
		{
			std::cout << "Brak elementu level" << std::endl;
			throw std::invalid_argument("No level node");
		}
		std::cout << root_name << ':' << std::endl;
		tinyxml2::XMLAttribute* att = (tinyxml2::XMLAttribute*)root->FirstAttribute();
		while (att != NULL)	//Przejœcie po wszytskich atrybutach g³ównego elementu
		{
			std::cout << att->Name() << ": " << att->Value() << std::endl;
			att = (tinyxml2::XMLAttribute*)att->Next();
		}
		tinyxml2::XMLElement* element = root->FirstChildElement();
		while (element != NULL)
		{
			att = (tinyxml2::XMLAttribute*)element->FirstAttribute();
			std::string name = element->Name();
			std::cout << name << ':' << std::endl;
			if (name == "platform")//Na razie tylko prostok¹tne
			{
				Vectorf pos = Vectorf(-1, -1);
				Vectorf size = Vectorf(-1, -1);//Niezbyt eleganckie sprawdzanie danych
				std::shared_ptr<sf::Texture> tex;
				while (att != NULL)
				{
					std::string att_name = att->Name();
					if (att_name == "position")
					{
						std::string val = att->Value();
						try
						{
							size_t p = val.find(',');
							if (p == std::string::npos)
							{
								throw std::invalid_argument("No ',' found");
							}
							int x = std::stoi(val.substr(0, p));
							int y = std::stoi(val.substr(p + 1));
							pos = Vectorf(x, y);
						}
						catch (const std::exception & e)
						{
							std::cout << "Wyjatek: " << e.what() << "\nElement: " << name << "\nAtrybut: " << att_name << std::endl;
						}
					}
					if (att_name == "size")
					{
						std::string val = att->Value();
						try
						{
							size_t p = val.find(',');
							if (p == std::string::npos)
							{
								throw std::invalid_argument("No ',' found");
							}
							int x = std::stoi(val.substr(0, p));
							int y = std::stoi(val.substr(p + 1));
							size = Vectorf(x, y);
						}
						catch (const std::exception & e)
						{
							std::cout << "Wyjatek: " << e.what() << "\nElement: " << name << "\nAtrybut: " << att_name << std::endl;
						}
					}
					if (att_name == "texture")
					{
						std::string val = att->Value();
						try
						{
							size_t p = val.find(',');
							if (p == std::string::npos)
							{
								throw std::invalid_argument("No ',' found");
							}
							std::string tex_name = val.substr(0, p);
							int tex_n = std::stoi(val.substr(p + 1));
							if (tex_name == "bricks")
								tex = std::make_shared<sf::Texture>(assets->bricks[tex_n]);
						}
						catch (const std::exception & e)
						{
							std::cout << "Wyjatek: " << e.what() << "\nElement: " << name << "\nAtrybut: " << att_name << std::endl;
						}
					}
					att = (tinyxml2::XMLAttribute*)att->Next();
				}
				if (tex != NULL && pos != Vectorf(-1, -1) && size != Vectorf(-1, -1))
				{
					std::vector<Vectorf> points;
					points.push_back(Vectorf(pos.x, pos.y));
					points.push_back(Vectorf(pos.x, pos.y + size.y));
					points.push_back(Vectorf(pos.x + size.x, pos.y + size.y));
					points.push_back(Vectorf(pos.x + size.x, pos.y));
					Platform plat = Platform(pos, tex, points);
					lvl.addTexturable(plat);
				}
			}
			while (att != NULL)
			{
				std::cout << att->Name() << ": " << att->Value() << std::endl;
				att = (tinyxml2::XMLAttribute*)att->Next();
			}
			element = element->NextSiblingElement();
		}
	}
	return lvl;
}
