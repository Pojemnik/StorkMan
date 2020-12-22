#pragma once
#include <queue>
#include <algorithm>
#include <variant>
#include <list>
#include <string>
#include <tuple>
#include <SFML/Graphics.hpp>

#include "map_sound_info.h"

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

class Message_sender;

enum class Message_sender_type {
	PLAYER = 0, MAP, SOUND_SYSTEM, INTERPRETER, ENGINE,
	ENEMY, CONSOLE, LEVEL, CHUNK
};

class Message_sender_id
{
	Message_sender_type type;
	int id;

public:
	Message_sender_id(Message_sender_type type_);
	Message_sender_type get_type() const;
	int get_type_int() const;
	int get_id() const;
};

struct Message
{
	enum class Message_type {
		DIED, DAMAGED, MOVED, JUMPED, ERROR, OUT, CHANGED_LEVEL, WINDOW_FOCUS,
		MUSIC_VOLUME, RESOLUTION_CHANGED, SOUND_VOLUME, STOPPED, ENTERED_SOUND,
		LEFT_SOUND, ADD_GRID_POINT, REMOVE_GRID_POINTS, WINDOW_CLOSED,
		CAMERA_MOVED, CONSOLE_ACTIVATED, CONSOLE_DEACTIVATED, WINDOW_RESIZED,
		CONSOLE_HISTORY, CONSOLE_INPUT, CONSOLE_COMMAND_RECEIVED, MOUSE_SCROLLED,
		RELOAD_MAP, DYING, MAP_EVENT, LOG, MOUSE_CLICKED
	};
	Message_type type;
	std::variant<int, std::string, float, bool, Vectori, Vectorf, Map_sound_info, char> args;
	const Message_sender* sender;

	Message(Message_type type_, const Message_sender* sender_);
	Message(Message_type type_, const Message_sender* sender_,
		std::variant<int, std::string, float, bool, Vectori, Vectorf, Map_sound_info, char> args_) :
		type(type_), sender(sender_), args(args_) {}
};

class Message_receiver
{
	std::queue<Message> msg_queue;

protected:
	bool message_available() const;
	Message pop_message();

public:
	virtual void push_message(Message& msg);
};

class Receiver_component : public Message_receiver
{
public:
	virtual void push_message(Message& msg);
	bool message_available() const;
	Message pop_message();
};

class Message_sender
{
	std::list<Message_receiver*> receivers;

public:
	Message_sender(Message_sender_type type);
	template <typename T>
	void send_message(Message::Message_type type, T args) const;
	void send_message(Message::Message_type type) const;
	void add_receiver(Message_receiver* receiver);
	void remove_receiver(Message_receiver* receiver);
	const Message_sender_id id;
};

template <typename T>
void Message_sender::send_message(Message::Message_type type, T args) const
{
	std::variant<int, std::string, float, bool, Vectori, Vectorf, Map_sound_info, char> args_variant = args;
	Message msg(type, this, args_variant);
	for (const auto& it : receivers)
	{
		it->push_message(msg);
	}
}