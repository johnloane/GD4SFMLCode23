#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.hpp"
#include "Texture.hpp"

class Game
{
public:
	Game(ResourceHolder<sf::Texture, Texture>& game_textures);
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time delta_time);
	void Render();
	void HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed);

private:
	static const float kPlayerSpeed;
	static const sf::Time kTimePerFrame;

	ResourceHolder<sf::Texture, Texture> & m_textures;

	sf::RenderWindow m_window;
	sf::Texture m_texture;
	sf::Sprite m_player;
	bool m_is_moving_up;
	bool m_is_moving_down;
	bool m_is_moving_left;
	bool m_is_moving_right;
};

