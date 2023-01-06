#include "GameState.hpp"
#include "Player.hpp"

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context)
    , m_world(*context.window, *context.fonts, *context.sounds)
    , m_player(*context.player)
{
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
        RequestStackPush(StateID::kGameOver);
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
