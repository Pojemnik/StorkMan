#include "event_handler.h"

Event_handler::Event_handler() : Message_sender(Message_sender_type::ENGINE) {}

void Event_handler::handle_event(sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		send_message(Message::Message_type::WINDOW_CLOSED, NULL);
		return;
	}
	if (!context.window_focus)
	{
		return;
	}
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Tilde)
		{
			if (context.console->is_active())
			{
				context.console->deactivate();
			}
			else
			{
				context.console->activate();
			}
		}
		if (context.console->is_active())
		{
			if (event.key.control && event.key.code == sf::Keyboard::V)
			{
				string s = sf::Clipboard::getString();
				context.console->input_append(s);
			}
			if (event.key.code == sf::Keyboard::Up)
			{
				context.console->get_next_history_line();
			}
			if (event.key.code == sf::Keyboard::Down)
			{
				context.console->get_previous_history_line();
			}
		}
		else
		{
			if (event.key.code == sf::Keyboard::G)
			{
				context.gravity = -context.gravity;
			}
			if (context.editor_mode)
			{
				if (event.key.code == sf::Keyboard::Delete)
				{
					if (!context.console->is_active())
					{
						send_message(Message::Message_type::REMOVE_GRID_POINTS, NULL);
					}
				}
			}
		}
		break;
	case sf::Event::TextEntered:
		if (context.console->is_active())
		{
			if (event.text.unicode < 128)
			{
				char c = char(event.text.unicode);
				context.console->input_append(c);
			}
		}
		break;
	case sf::Event::MouseWheelScrolled:
		if (context.console->is_active())
		{
			context.console->scroll((int)event.mouseWheelScroll.delta);
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (!context.console->is_active())
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (context.editor_mode)
				{
					send_message(Message::Message_type::ADD_GRID_POINT, NULL);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (context.editor_mode)
				{
					camera_move = true;
					mouse_start_pos = static_cast<Vectorf>(sf::Mouse::getPosition());
				}
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Right
			&& !context.console->is_active())
		{
			camera_move = false;
		}
		break;
	case sf::Event::MouseMoved:
		if (camera_move && context.editor_mode)
		{
			Vectorf mouse_pos = static_cast<Vectorf>(sf::Mouse::getPosition());
			Vectorf mouse_delta = mouse_pos - mouse_start_pos;
			mouse_start_pos = mouse_pos;
			//mouse_pos = Vectorf(mouse_pos.x / context.camera_zoom.x,
			//	mouse_pos.y / context.camera_zoom.y);
			send_message<Vectorf>(Message::Message_type::CAMERA_MOVED, mouse_delta);
		}
		break;
	}
}
