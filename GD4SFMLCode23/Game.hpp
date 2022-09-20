#pragma once
#include <SFML/Graphics.hpp>
class Game
{
public:
	Game();
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time delta_time);
	void Render();
	void HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed);

private:
	static const float kPlayerSpeed;
	static const sf::Time kTimePerFrame;

	sf::RenderWindow m_window;
	sf::CircleShape m_player;
	bool m_is_moving_up;
	bool m_is_moving_down;
	bool m_is_moving_left;
	bool m_is_moving_right;
};

