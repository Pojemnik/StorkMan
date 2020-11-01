#include "sound.h"

Sound_system::Sound_system(const std::vector<sf::SoundBuffer>* buffers_, std::vector<string> music_paths_) :
	buffers(buffers_), music_paths(music_paths_), Message_sender(Message_sender_type::SOUND_SYSTEM)
{
	music.setLoop(true);
}

void Sound_system::update(float dt)
{
	while (message_available())
	{
		Message msg = pop_message();
		try
		{
			if (msg.type == Message::Message_type::CHANGED_LEVEL)
			{
				int id = std::get<int>(msg.args);
				if (music_id == -1 || music_paths.at(id) != music_paths.at(music_id))
				{
					timer = CHANGE_DELTA;
					next_music_id = id;
					state = Music_state::QUIETER;
				}
			}
		}
		catch (const std::out_of_range& e)
		{
			send_message(Message::Message_type::ERROR, "Error while playing sound! " + static_cast<string>(e.what()));
		}
	}
	if (state == Music_state::LOUDER)
	{
		timer -= dt;
		if (timer <= 0)
		{
			state = Music_state::DEFAULT;
			music.setVolume(100);
		}
		else
		{
			music.setVolume(100.f - ((timer / CHANGE_DELTA) * 100.f));
		}
	}
	if (state == Music_state::QUIETER)
	{
		timer -= dt;
		if (timer <= 0)
		{
			try
			{
				music_id = next_music_id;
				music.openFromFile(music_paths.at(music_id));
				music.play();
				state = Music_state::LOUDER;
				timer = CHANGE_DELTA;
			}
			catch (const std::out_of_range& e)
			{
				send_message(Message::Message_type::ERROR, "Error while playing sound! " + static_cast<string>(e.what()));
			}
		}
		else
		{
			music.setVolume((timer / CHANGE_DELTA) * 100.f);
		}
	}
}
