#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>

struct AircraftMover
{
    AircraftMover(float vx, float vy)
        : velocity(vx, vy)
    {}
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        aircraft.Accelerate(velocity);
    }
    sf::Vector2f velocity;
};

Player::Player()
{
    //Set initial key bindings
    m_key_binding[sf::Keyboard::Left] = Action::kMoveLeft;
    m_key_binding[sf::Keyboard::Right] = Action::kMoveRight;
    m_key_binding[sf::Keyboard::Up] = Action::kMoveUp;
    m_key_binding[sf::Keyboard::Down] = Action::kMoveDown;

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
}

void Player::HandleRealtimeInput(CommandQueue& command)
{
}

void Player::AssignKey(Action action, sf::Keyboard::Key key)
{
}

sf::Keyboard::Key Player::GetAssignedKey(Action action) const
{
    return sf::Keyboard::Key();
}

void Player::InitializeAction()
{
}

bool Player::IsRealtimeAction(Action action)
{
    return false;
}
