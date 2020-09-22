#include "messaging.h"

bool Message_receiver::message_available() const
{
	return msg_queue.size() > 0;
}

Message Message_receiver::pop_message()
{
	Message msg = msg_queue.front();
	msg_queue.pop();
	return msg;
}

void Message_receiver::push_message(Message& msg)
{
	msg_queue.push(msg);
}

template <typename T>
void Message_sender::send_message(Message::Message_type type, T args) const
{
	std::variant<T> args_variant = args;
	Message msg(type, this, args_variant);
	for (const auto& it : receivers)
	{
		it->push_message(msg);
	}
}

void Message_sender::add_receiver(Message_receiver* receiver)
{
	receivers.push_back(receiver);
}

void Message_sender::remove_receiver(Message_receiver* receiver)
{
	std::remove_if(receivers.begin(), receivers.end(), receiver);
}

Message::Message(Message_type type_, Message_sender* sender_) :
	type(type_), sender(sender_) {}
