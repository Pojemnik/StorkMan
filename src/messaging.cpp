#include "messaging.h"

Message_sender_id::Message_sender_id(Message_sender_type type_) : type(type_)
{
	static int next_id(0);
	id = next_id++;
}

Message_sender_type Message_sender_id::get_type() const
{
	return type;
}

int Message_sender_id::get_id() const
{
	return id;
}

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

Message_sender::Message_sender(Message_sender_type type) : id(type)
{

}

void Message_sender::add_receiver(Message_receiver* receiver)
{
	receivers.push_back(receiver);
}

void Message_sender::remove_receiver(Message_receiver* receiver)
{
	receivers.remove(receiver);
}

Message::Message(Message_type type_, const Message_sender* sender_) :
	type(type_), sender(sender_) {}
