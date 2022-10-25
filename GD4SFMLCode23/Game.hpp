#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "World.hpp"
#include "Player.hpp"


class Game : private sf::NonCopyable
{
public:
	Game();
	void Run();

private:
	void HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed);
	void ProcessInput();
	void Update(sf::Time elapsed_time);
	void Render();



private:
	static const sf::Time kTimePerFrame;
	//The order of things is essential here. If you declare the world before the window, world will get constructed before a RenderWindow has been constructed and your world will be trying to use a window which hasn't been constrcuted yet and hence your worlds window, camera and spawnposition will be a mess
	sf::RenderWindow m_window;
	World m_world;
	Player m_player;
};

