#pragma once
#include "messaging.h"

class Event_handler : public Message_sender
{
	
public:
	Event_handler();
	void handle_event(sf::Event& event);
};