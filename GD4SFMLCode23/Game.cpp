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
		case sf::Event::Closed:
			m_window.close();
			break;
		}
		
	}
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

