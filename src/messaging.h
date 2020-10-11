#pragma once
#include <queue>
#include <vector>
#include <algorithm>
#include <variant>

class Message_sender;

struct Message
{
	enum class Message_type { DIED, DAMAGED, MOVED, JUMPED } type;
	std::variant<int> args;
	const Message_sender* sender;

	Message(Message_type type_, const Message_sender* sender_);
	Message(Message_type type_, const Message_sender* sender_, std::variant<int> args_) :
		type(type_), sender(sender_), args(args_) {}
};

class Message_receiver
{
	std::queue<Message> msg_queue;

protected:
	bool message_available() const;
	Message pop_message();

public:
	void push_message(Message& msg);
};

class Message_sender
{
	std::vector<Message_receiver*> receivers;

protected:
	void add_receiver(Message_receiver* receiver);
	void remove_receiver(Message_receiver* receiver);

public:
	void send_message(Message::Message_type type, int args) const;
};