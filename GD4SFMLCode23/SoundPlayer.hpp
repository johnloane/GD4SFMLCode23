#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
public:
	SoundPlayer();

	void Play(SoundEffect effect);
	void Play(SoundEffect effect, sf::Vector2f position);

	void RemoveStoppedSounds();
	void SetListenerPosition(sf::Vector2f position);
	sf::Vector2f GetListenerPosition() const;


private:
	SoundBufferHolder m_sound_buffers;
	std::list<sf::Sound> m_sounds;
};


