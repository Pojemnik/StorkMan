#pragma once
#include <queue>
#include <algorithm>
#include <variant>
#include <list>
#include <string>

class Message_sender;

enum class Message_sender_type { PLAYER, MAP, SOUND_SYSTEM, INTERPRETER };

class Message_sender_id
{	
	Message_sender_type type;
	int id;

public:
	Message_sender_id(Message_sender_type type_);
	Message_sender_type get_type() const;
	int get_id() const;
};

struct Message
{
	enum class Message_type { DIED, DAMAGED, MOVED, JUMPED, ERROR, OUT, CHANGED_LEVEL, MUSIC_VOLUME } type;
	std::variant<int, std::string, float> args;
	const Message_sender* sender;

	Message(Message_type type_, const Message_sender* sender_);
	Message(Message_type type_, const Message_sender* sender_,
		std::variant<int, std::string, float> args_) :
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

class Message_sender
{
	std::list<Message_receiver*> receivers;

public:
	Message_sender(Message_sender_type type);
	template <typename T>
	void send_message(Message::Message_type type, T args) const;
	void add_receiver(Message_receiver* receiver);
	void remove_receiver(Message_receiver* receiver);
	const Message_sender_id id;
};

template <typename T>
void Message_sender::send_message(Message::Message_type type, T args) const
{
	std::variant<int, std::string, float> args_variant = args;
	Message msg(type, this, args_variant);
	for (const auto& it : receivers)
	{
		it->push_message(msg);
	}
}