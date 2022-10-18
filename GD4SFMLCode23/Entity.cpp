#include "Entity.hpp"

void Entity::SetVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::SetVelocity(float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f Entity::GetVelocity() const
{
    return m_velocity;
}

void Entity::Accelerate(sf::Vector2f velocity)
{
    m_velocity += velocity;
}

void Entity::Accelerate(float vx, float vy)
{
    m_velocity.x += vx;
    m_velocity.y += vy;
}

void Entity::UpdateCurrent(sf::Time dt)
{
    move(m_velocity * dt.asSeconds());
}
