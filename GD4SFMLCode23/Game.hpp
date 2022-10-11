#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.hpp"
#include "Texture.hpp"
#include "World.hpp"

class Game
{
public:
	Game();
	void Run();


private:
	World m_world;
	sf::RenderWindow m_window;
	static const sf::Time kTimePerFrame;

};

