#include "sound.h"

Sound_system::Sound_system(const std::unordered_map<int, std::vector<string>>& entity_sounds_paths,
	const std::vector<string>& music_paths_,
	const std::vector<string>& map_sounds_paths,
	const std::unordered_map<int, string> steps_config) :
	music_paths(music_paths_), Message_sender(Message_sender_type::SOUND_SYSTEM)
{
	music.setLoop(true);
	for (const auto& it : entity_sounds_paths)
	{
		entity_sounds.insert({ it.first, {} });
		for (const auto& it2 : it.second)
		{
			sf::SoundBuffer sb;
			if (!sb.loadFromFile(it2))
				throw std::invalid_argument("Incorrect sound path!");
			entity_sounds.at(it.first).push_back(sb);
		}
	}
	for (const auto& it : map_sounds_paths)
	{
		sf::SoundBuffer sb;
		if (!sb.loadFromFile(it))
			throw std::invalid_argument("Incorrect sound path!");
		map_sounds.push_back(sb);
	}
	for (const auto& it : steps_config)
	{
		sf::SoundBuffer sb;
		if (!sb.loadFromFile(it.second))
			throw std::invalid_argument("Incorrect step sound path!");
		surface_sounds.insert({ it.first, sb });
	}
}

void Sound_system::update(float dt)
{
	sf::Listener::setPosition(context.player_pos.x / context.global_scale,
		context.player_pos.y / context.global_scale, 0);
	while (message_available())
	{
		Message msg = pop_message();
		if (message_function.contains(msg.type))
		{
			try
			{
				message_function.at(msg.type)(msg);
			}
			catch (const std::out_of_range& e)
			{
				send_message(Msgtype::ERROR, "Error while playing sound! " + static_cast<string>(e.what()) + "\n");
			}
		}
	}
	if (!muted)
	{
		update_music_state(dt);
	}
}

void Sound_system::on_music_volume_change(const Message& msg)
{
	music_volume = std::get<int>(msg.args);
	music.setVolume(music_volume);
}

void Sound_system::on_entity_jump(const Message& msg)
{
	if (std::get<bool>(msg.args))
	{
		pool.play_sound(entity_sounds.at(msg.sender->id.get_type_int())[1]);
	}
	else
	{
		pool.play_sound(entity_sounds.at(msg.sender->id.get_type_int())[0]);
	}
}

void Sound_system::on_entity_death(const Message& msg)
{
	pool.play_sound(entity_sounds.at(msg.sender->id.get_type_int())[2]);
}

void Sound_system::on_sound_volume_change(const Message& msg)
{
	sound_volume = std::get<int>(msg.args);
	pool.set_volume(sound_volume);
	for (auto& it : steps)
	{
		it.second.setVolume(sound_volume);
	}
}

void Sound_system::on_entity_move(const Message& msg)
{
	int sender_type = msg.sender->id.get_type_int();
	if (!steps.contains(sender_type))
	{
		steps.insert({ sender_type, sf::Sound() });
		steps.at(sender_type).setLoop(true);
	}
	int surface_type = std::get<int>(msg.args);
	if (surface_sounds.contains(surface_type))
	{
		steps.at(sender_type).setBuffer(surface_sounds.at(surface_type));
		steps.at(sender_type).play();
	}
}

void Sound_system::on_entity_stop(const Message& msg)
{
	int sender_type = msg.sender->id.get_type_int();
	if (steps.contains(sender_type))
	{
		steps.at(sender_type).stop();
	}
}

void Sound_system::on_sound_enter(const Message& msg)
{
	auto args = std::get<std::tuple<int, int, Vectorf>>(msg.args);
	pool.play_map_sound(map_sounds.at(std::get<0>(args)),
		std::make_pair(lvl_id, std::get<1>(args)), std::get<2>(args));
}

void Sound_system::on_sound_left(const Message& msg)
{
	pool.stop_sound(std::make_pair(lvl_id, std::get<1>(std::get<std::tuple<int, int, Vectorf>>(msg.args))));
}

void Sound_system::on_window_focus_change(const Message& msg)
{
	if (std::get<bool>(msg.args))
	{
		muted = false;
		music.play();
	}
	else
	{
		muted = true;
		music.pause();
	}
}

void Sound_system::on_level_change(const Message& msg)
{
	int id = std::get<int>(msg.args);
	lvl_id = id;
	if (music_id == -1 || music_paths.at(id) != music_paths.at(music_id))
	{
		timer = CHANGE_DELTA;
		next_music_id = id;
		if (state == Music_state::UNINITALIZED)
		{
			state = Music_state::LOUDER;
			music_id = id;
			music.openFromFile(music_paths.at(music_id));
			music.play();
		}
		else
		{
			state = Music_state::QUIETER;
		}
	}
}

void Sound_system::update_music_state(float dt)
{
	if (state == Music_state::LOUDER)
	{
		timer -= dt;
		if (timer <= 0)
		{
			state = Music_state::DEFAULT;
			music.setVolume(music_volume);
		}
		else
		{
			float vol = (1.f - (timer / CHANGE_DELTA)) * music_volume;
			music.setVolume(vol);
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
			music.setVolume((timer / CHANGE_DELTA) * music_volume);
		}
	}
}

int Sound_pool::play_sound(const sf::SoundBuffer& sb, bool relative, Vectorf pos, bool repeated)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (pool[i].getStatus() == sf::SoundSource::Status::Stopped)
		{
			pool[i].setBuffer(sb);
			pool[i].setRelativeToListener(relative);
			pool[i].setPosition(pos.x, pos.y, 0);
			pool[i].setLoop(repeated);
			pool[i].play();
			return i;
		}
	}
	throw std::runtime_error("Sound pool overflow");
}

void Sound_pool::play_map_sound(const sf::SoundBuffer& sb, std::pair<int, int> id, Vectorf pos)
{
	map_sounds.insert({ id, play_sound(sb, false, pos, true) });
}

void Sound_pool::set_volume(int volume)
{
	for (auto& it : pool)
	{
		it.setVolume(volume);
	}
}

void Sound_pool::stop_sound(std::pair<int, int> id)
{
	int sound_i = map_sounds.at(id);
	map_sounds.erase(id);
	pool[sound_i].stop();
}
