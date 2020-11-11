#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <functional>
#include <array>

#include "messaging.h"

typedef std::string string;
typedef Message::Message_type Msgtype;

class Sound_pool
{
	static const int POOL_SIZE = 30;
	std::array<sf::Sound, POOL_SIZE> pool;

public:
	void play_sound(const sf::SoundBuffer& sb);
	void set_volume(int volume);
};

class Sound_system : public Message_receiver, public Message_sender
{
	enum class Music_state { LOUDER, QUIETER, DEFAULT, UNINITALIZED };

	Sound_pool pool;
	std::unordered_map<int, std::vector<sf::SoundBuffer>> entity_sounds;
	const std::vector<string> music_paths;
	std::unordered_map<int, sf::Sound> steps;
	sf::Music music;
	int music_id = -1;
	int next_music_id = -1;
	const float CHANGE_DELTA = 180.f;
	float timer = 0.f;
	Music_state state = Music_state::UNINITALIZED;
	int music_volume = 100;
	int sound_volume = 100;
	bool muted = false;
	const std::unordered_map<Msgtype, std::function<void(const Message&)>>
		message_function = 
	{
		{Msgtype::CHANGED_LEVEL, std::bind(&Sound_system::on_level_change, this, std::placeholders::_1)},
		{Msgtype::WINDOW_FOCUS, std::bind(&Sound_system::on_window_focus_change, this, std::placeholders::_1)},
		{Msgtype::MUSIC_VOLUME, std::bind(&Sound_system::on_music_volume_change, this, std::placeholders::_1)},
		{Msgtype::JUMPED, std::bind(&Sound_system::on_entity_jump, this, std::placeholders::_1)},
		{Msgtype::DIED, std::bind(&Sound_system::on_entity_death, this, std::placeholders::_1)},
		{Msgtype::SOUND_VOLUME, std::bind(&Sound_system::on_sound_volume_change, this, std::placeholders::_1)}
	};

	void update_music_state(float dt);
	void on_level_change(const Message& msg);
	void on_window_focus_change(const Message& msg);
	void on_music_volume_change(const Message& msg);
	void on_entity_jump(const Message& msg);
	void on_entity_death(const Message& msg);
	void on_sound_volume_change(const Message& msg);

public:
	Sound_system(std::unordered_map<int, std::vector<string>> entity_sounds_paths_, std::vector<string> music_paths_);
	void update(float dt);
};