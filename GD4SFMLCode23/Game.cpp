#include "Game.hpp"

const float Game::kPlayerSpeed = 100.f;
const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: m_window(sf::VideoMode(640, 480), "Getting started")
	, m_player(), m_is_moving_up(false), m_is_moving_down(false), m_is_moving_left(false), m_is_moving_right(false)
{
	m_player.setRadius(40.f);
	m_player.setPosition(100.f, 100.f);
	m_player.setFillColor(sf::Color::Cyan);
}

void Game::Run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while (m_window.isOpen())
	{
		time_since_last_update += clock.restart();
		if (time_since_last_update > kTimePerFrame)
		{
			time_since_last_update -= kTimePerFrame;
			ProcessEvents();
			Update(kTimePerFrame);
		}
		
		Render();
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			HandlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			HandlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			m_window.close();
			break;
		}
		
	}
}

void Game::Update(sf::Time delta_time)
{
	sf::Vector2f movement(0.f, 0.f);
	if (m_is_moving_up)
	{
		movement.y -= kPlayerSpeed;
	}
	if (m_is_moving_down)
	{
		movement.y += kPlayerSpeed;
	}
	if (m_is_moving_left)
	{
		movement.x -= kPlayerSpeed;
	}
	if (m_is_moving_right)
	{
		movement.x += kPlayerSpeed;
	}

	m_player.move(movement*delta_time.asSeconds());
}

void Game::Render()
{
	m_window.clear();
	m_window.draw(m_player);
	m_window.display();
}

void Game::HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed)
{
	if (key == sf::Keyboard::W)
	{
		m_is_moving_up = is_pressed;
	}
	else if (key == sf::Keyboard::S)
	{
		m_is_moving_down = is_pressed;
	}
	else if (key == sf::Keyboard::A)
	{
		m_is_moving_left = is_pressed;
	}
	else if (key == sf::Keyboard::D)
	{
		m_is_moving_right = is_pressed;
	}
}
