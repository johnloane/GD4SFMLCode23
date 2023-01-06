#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
	: m_volume(100.f)
{
	m_filenames[MusicThemes::kMenuTheme] = "Media/Music/MenuTheme.ogg";
	m_filenames[MusicThemes::kMissionTheme] = "Media/Music/MissionTheme.ogg";
}

void MusicPlayer::Play(MusicThemes theme)
{
	std::string filename = m_filenames[theme];

	if (!m_music.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	m_music.setVolume(m_volume);
	m_music.setLoop(true);
	m_music.play();
}

void MusicPlayer::Stop()
{
	m_music.stop();
}

void MusicPlayer::SetVolume(float volume)
{
	m_volume = volume;
}

void MusicPlayer::SetPaused(bool paused)
{
	if (paused)
		m_music.pause();
	else
		m_music.play();
}