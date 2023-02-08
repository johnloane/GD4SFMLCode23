#include "Application.hpp"

#include "TitleState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "GameOverState.hpp"
#include "MultiplayerGameState.hpp"

const sf::Time Application::kTimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: m_window(sf::VideoMode(1024, 768), "Networked", sf::Style::Close)
	, m_key_binding_1(1), m_key_binding_2(2)
	, m_stack(State::Context(m_window, m_textures, m_fonts, m_music, m_sound, m_key_binding_1, m_key_binding_2))
{
	m_window.setKeyRepeatEnabled(false);

	m_fonts.Load(Font::kMain, "Media/Fonts/Sansation.ttf");
	m_textures.Load(Texture::kTitleScreen, "Media/Textures/TitleScreen.png");
	m_textures.Load(Texture::kButtons, "Media/Textures/Buttons.png");


	RegisterStates();
	m_stack.PushState(StateID::kTitle);
}

void Application::Run()
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

			if (m_stack.IsEmpty())
			{
				m_window.close();
			}
		}

		Render();
	}
}

void Application::ProcessInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_stack.HandleEvent(event);
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Application::Update(sf::Time dt)
{
	m_stack.Update(dt);
}

void Application::Render()
{
	m_window.clear();
	m_stack.Draw();
	m_window.display();
}

void Application::RegisterStates()
{
	m_stack.RegisterState<TitleState>(StateID::kTitle);
	m_stack.RegisterState<MenuState>(StateID::kMenu);
	m_stack.RegisterState<GameState>(StateID::kGame);
	m_stack.RegisterState<MultiplayerGameState>(StateID::kHostGame, true);
	m_stack.RegisterState<MultiplayerGameState>(StateID::kJoinGame, false);
	m_stack.RegisterState<PauseState>(StateID::kPause);
	m_stack.RegisterState<PauseState>(StateID::kNetworkPause, true);
	m_stack.RegisterState<SettingsState>(StateID::kSettings);
	m_stack.RegisterState<GameOverState>(StateID::kGameOver, "Mission Failed!");
	m_stack.RegisterState<GameOverState>(StateID::kMissionSuccess, "Mission Successful!");
}
