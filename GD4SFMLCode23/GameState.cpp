#include "GameState.hpp"
#include "Player.hpp"
#include <iostream>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context)
    , m_world(*context.window, *context.fonts, *context.sounds, false)
    , m_player(nullptr, 1, context.keys1)
{
    m_world.AddAircraft(1);
    m_player.SetMissionStatus(MissionStatus::kMissionRunning);
    // Play game theme
    context.music->Play(MusicThemes::kMissionTheme);
}

void GameState::Draw()
{
    m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{
    m_world.Update(dt);
    if (!m_world.HasAlivePlayer())
    {
        m_player.SetMissionStatus(MissionStatus::kMissionFailure);
        RequestStackPush(StateID::kGameOver);
    }
    else if (m_world.HasPlayerReachedEnd())
    {
        m_player.SetMissionStatus(MissionStatus::kMissionSuccess);
        RequestStackPush(StateID::kMissionSuccess);
    }
    CommandQueue& commands = m_world.GetCommandQueue();
    m_player.HandleRealtimeInput(commands);
    return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
    CommandQueue& commands = m_world.GetCommandQueue();
    m_player.HandleEvent(event, commands);

    //Escape should bring up the pause menu
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        RequestStackPush(StateID::kPause);
    }
    return true;
}
