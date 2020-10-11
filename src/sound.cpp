#include "sound.h"

void Sound_system::update()
{
	while (message_available())
	{
		Message msg = pop_message();
		//Parse message and play appropriate sound
	}
}
