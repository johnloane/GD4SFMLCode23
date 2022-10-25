#include "Game.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: m_window(sf::VideoMode(640, 480), "Getting started")
	, m_world(m_window)	
{
}

void Game::Run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while (m_window.isOpen())
	{
		time_since_last_update += clock.restart();
		while (time_since_last_update > kTimePerFrame)
		{
			time_since_last_update -= kTimePerFrame;
			ProcessInput();
			Update(kTimePerFrame);
		}
		
		Render();
	}
}

void Game::ProcessInput()
{
	CommandQueue& commands = m_world.GetCommandQueue();

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_player.HandleEvent(event, commands);
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
	m_player.HandleRealtimeInput(commands);
}




void Game::Update(sf::Time delta_time)
{
	m_world.Update(delta_time);
}

void Game::Render()
{
	m_window.clear();
	m_world.Draw();
	m_window.display();
}

