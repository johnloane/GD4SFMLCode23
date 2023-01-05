#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "ParticleType.hpp"

struct Particle
{
	ParticleType m_type;
	sf::Vector2f m_position;
	sf::Color m_colour;
	sf::Time m_lifetime;
};