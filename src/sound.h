#pragma once
#include <SFML/Audio.hpp>
#include "messaging.h"

class Sound_system : public Message_receiver
{
	std::vector<sf::Sound*> sounds;
public:
	void update();
};