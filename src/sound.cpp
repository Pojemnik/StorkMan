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
	music.setVolume(static_cast<float>(music_volume));
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
	int sender_type = msg.sender->id.get_type_int();
	if (steps.contains(sender_type))
	{
		steps.at(sender_type).stop();
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
		it.second.setVolume(static_cast<float>(sound_volume));
	}
}

void Sound_system::on_entity_move(const Message& msg)
{
	int sender_type = msg.sender->id.get_type_int();
	if (!steps.contains(sender_type))
	{
		steps.insert({ sender_type, sf::Sound() });
		steps.at(sender_type).setLoop(true);
		steps.at(sender_type).setVolume(static_cast<float>(sound_volume));
		steps.at(sender_type).setRelativeToListener(true);
	}
	int surface_type = std::get<int>(msg.args);
	if (surface_sounds.contains(surface_type))
	{
		steps.at(sender_type).setBuffer(surface_sounds.at(surface_type));
		steps.at(sender_type).play();
	}
	else
	{
		steps.at(sender_type).stop();
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
	Map_sound_info args = std::get<Map_sound_info>(msg.args);
	args.pos /= context.global_scale;
	pool.play_map_sound(map_sounds.at(args.sound), args, lvl_id);
}

void Sound_system::on_sound_left(const Message& msg)
{
	int sound_id = std::get<Map_sound_info>(msg.args).id;
	pool.stop_sound({lvl_id, sound_id});
}

void Sound_system::on_map_reload(const Message& msg)
{
	pool.stop_all();
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
	lvl_id = std::get<int>(msg.args);
	if (music_id == -1 || music_paths.at(lvl_id) != music_paths.at(music_id))
	{
		timer = CHANGE_DELTA;
		next_music_id = lvl_id;
		if (state == Music_state::UNINITALIZED)
		{
			state = Music_state::LOUDER;
			music_id = lvl_id;
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
			music.setVolume(static_cast<float>(music_volume));
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

Sound_pool::Sound_pool()
{
	for (auto& it : pool)
	{
		it.stop();
	}
}

int Sound_pool::play_sound(const sf::SoundBuffer& sb)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (pool[i].is_stopped())
		{
			pool[i].play(sb, 100);
			return i;
		}
	}
	throw std::runtime_error("Sound pool overflow");
}

int Sound_pool::play_sound(const sf::SoundBuffer& sb, Map_sound_info info)
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (pool[i].is_stopped())
		{
			pool[i].play(sb, info);
			return i;
		}
	}
	throw std::runtime_error("Sound pool overflow");
}

void Sound_pool::play_map_sound(const sf::SoundBuffer& sb, Map_sound_info info, int lvl_id)
{
	map_sounds.insert({ {lvl_id, info.id}, play_sound(sb, info) });
}

void Sound_pool::set_volume(int volume)
{
	for (auto& it : pool)
	{
		it.set_volume(volume);
	}
}

void Sound_pool::stop_sound(std::pair<int, int> id)
{
	int sound_i = map_sounds.at(id);
	map_sounds.erase(id);
	pool[sound_i].stop();
}

void Sound_pool::stop_all()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		pool[i].stop();
	}
	map_sounds.clear();
}

void Sound::reset()
{
	sound.setLoop(false);
	sound.setMinDistance(1.f);
	sound.setPosition(0.f,0.f,0.f);
	sound.setRelativeToListener(true);
	sound.setVolume(100);
	sound_volume = 100;
}

void Sound::play(const sf::SoundBuffer& buffer, int volume)
{
	reset();
	sound_volume = volume;
	sound.setVolume((float)system_volume * sound_volume / 100.f);
	sound.setBuffer(buffer);
	sound.play();
}

void Sound::play(const sf::SoundBuffer& buffer, Map_sound_info info)
{
	sound.setAttenuation(info.attenuation);
	sound.setLoop(info.repeat);
	sound.setMinDistance(info.min_distance);
	sound.setPosition(info.pos.x, info.pos.y, 0);
	sound.setRelativeToListener(info.relative);
	sound_volume = info.volume;
	sound.setVolume((float)system_volume * sound_volume / 100.f);
	sound.setBuffer(buffer);
	sound.play();
}

void Sound::set_volume(int new_volume)
{
	system_volume = new_volume;
	sound.setVolume((float)system_volume * sound_volume / 100.f);
}

void Sound::stop()
{
	sound.stop();
	reset();
}

bool Sound::is_stopped() const
{
	return (sound.getStatus() == sf::SoundSource::Status::Stopped);
}
