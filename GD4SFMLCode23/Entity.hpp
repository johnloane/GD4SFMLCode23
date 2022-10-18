#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	sf::Vector2f GetVelocity() const;
	void Accelerate(sf::Vector2f velocity);
	void Accelerate(float vx, float vy);

private:
	virtual void UpdateCurrent(sf::Time dt);

private:
	sf::Vector2f m_velocity;
};

