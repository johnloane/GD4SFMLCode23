#pragma once
#include "Entity.hpp"
#include "AircraftType.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity
{
public:
	Aircraft(AircraftType type, const TextureHolder& textures);
	unsigned int GetCategory() const override;

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	AircraftType m_type;
	sf::Sprite m_sprite;
};

