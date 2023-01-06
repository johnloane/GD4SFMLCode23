#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

#include "MusicThemes.hpp"


class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();

	void Play(MusicThemes theme);
	void Stop();

	void SetPaused(bool paused);
	void SetVolume(float volume);


private:
	sf::Music m_music;
	std::map<MusicThemes, std::string>	m_filenames;
	float m_volume;
};


