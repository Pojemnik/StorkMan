#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <functional>
#include "messaging.h"

typedef std::string string;
typedef Message::Message_type Msgtype;

class Sound_system : public Message_receiver, public Message_sender
{
	enum class Music_state { LOUDER, QUIETER, DEFAULT, UNINITALIZED };

	std::vector<std::unique_ptr<sf::Sound>> sounds;
	const std::vector<sf::SoundBuffer>* buffers;
	const std::vector<string> music_paths;
	sf::Music music;
	int music_id = -1;
	int next_music_id = -1;
	const float CHANGE_DELTA = 180.f;
	float timer;
	Music_state state = Music_state::UNINITALIZED;
	int music_volume = 100;
	bool muted = false;
	const std::unordered_map<Msgtype, std::function<void(const Message&)>>
		message_function = 
	{
		{Msgtype::CHANGED_LEVEL, std::bind(&Sound_system::on_level_change, this, std::placeholders::_1)},
		{Msgtype::WINDOW_FOCUS, std::bind(&Sound_system::on_window_focus_change, this, std::placeholders::_1)},
		{Msgtype::MUSIC_VOLUME, std::bind(&Sound_system::on_music_volume_change, this, std::placeholders::_1)}
	};

	void update_music_state(float dt);
	void on_level_change(const Message& msg);
	void on_window_focus_change(const Message& msg);
	void on_music_volume_change(const Message& msg);

public:
	Sound_system(const std::vector<sf::SoundBuffer>* buffers_, std::vector<string> music_paths_);
	void update(float dt);
};