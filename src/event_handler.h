#pragma once
#include "messaging.h"
#include "util.h"

class Event_handler : public Message_sender
{
	bool camera_move = false;
	Vectorf mouse_start_pos;
public:
	Event_handler();
	void handle_event(sf::Event& event);
};