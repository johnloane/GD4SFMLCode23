#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include "Player.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "StateStack.hpp"

#include "MusicPlayer.hpp"

class Application
{
public:
	Application();
	void Run();

private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();
	void RegisterStates();

private:
	sf::RenderWindow m_window;
	Player m_player;

	TextureHolder m_textures;
	FontHolder m_fonts;

	StateStack m_stack;
	static const sf::Time kTimePerFrame;

	MusicPlayer m_music;
};

