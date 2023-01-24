#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct AircraftMover
{
    AircraftMover(float vx, float vy)
        : velocity(vx, vy)
    {}
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        aircraft.Accelerate(velocity * aircraft.GetMaxSpeed());
    }
    sf::Vector2f velocity;
};

Player::Player() : m_current_mission_status(MissionStatus::kMissionRunning)
{
    //Set initial key bindings
    m_key_binding[sf::Keyboard::Left] = Action::kMoveLeft;
    m_key_binding[sf::Keyboard::Right] = Action::kMoveRight;
    m_key_binding[sf::Keyboard::Up] = Action::kMoveUp;
    m_key_binding[sf::Keyboard::Down] = Action::kMoveDown;
    m_key_binding[sf::Keyboard::M] = Action::kMissileFire;
    m_key_binding[sf::Keyboard::Space] = Action::kBulletFire;

    //Set initial action bindings
    InitializeActions();

    //Assign all categories to a player's aircraft
    for (auto& pair : m_action_binding)
    {
        pair.second.category = static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
    }

}


void Player::HandleEvent(const sf::Event& event, CommandQueue& command)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = m_key_binding.find(event.key.code);
        if (found != m_key_binding.end() && !IsRealtimeAction(found->second))
        {
            command.Push(m_action_binding[found->second]);
        }
    }
}

void Player::HandleRealtimeInput(CommandQueue& command)
{
    //Check if any of the key binding keys are pressed
    for (auto pair : m_key_binding)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && IsRealtimeAction(pair.second))
        {
            command.Push(m_action_binding[pair.second]);
        }
    }
}

void Player::AssignKey(Action action, sf::Keyboard::Key key)
{
    //Remove all keys that are currently bound to the action
    for (auto itr = m_key_binding.begin(); itr != m_key_binding.end();)
    {
        if (itr->second == action)
        {
            m_key_binding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    m_key_binding[key] = action;
}

sf::Keyboard::Key Player::GetAssignedKey(Action action) const
{
    for (auto pair : m_key_binding)
    {
        if (pair.second == action)
        {
            return pair.first;
        }
    }
    return sf::Keyboard::Unknown;
}

void Player::SetMissionStatus(MissionStatus status)
{
    m_current_mission_status = status;
}

MissionStatus Player::GetMissionStatus() const
{
    return m_current_mission_status;
}

void Player::InitializeActions()
{
    const float kPlayerSpeed = 200.f;
    m_action_binding[Action::kMoveLeft].action = DerivedAction<Aircraft>(AircraftMover(-1, 0.f));
    m_action_binding[Action::kMoveRight].action = DerivedAction<Aircraft>(AircraftMover(+1, 0.f));
    m_action_binding[Action::kMoveUp].action = DerivedAction<Aircraft>(AircraftMover(0.f, -1));
    m_action_binding[Action::kMoveDown].action = DerivedAction<Aircraft>(AircraftMover(0.f, 1));
    m_action_binding[Action::kBulletFire].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
    {
        a.Fire();
    });

    m_action_binding[Action::kMissileFire].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
    {
        a.LaunchMissile();
    });
}

bool Player::IsRealtimeAction(Action action)
{
    switch (action)
    {
    case Action::kMoveDown:
    case Action::kMoveUp:
    case Action::kMoveLeft:
    case Action::kMoveRight:
    case Action::kBulletFire:
        return true;
    default:
        return false;
    }
}
