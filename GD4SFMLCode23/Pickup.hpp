#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "PickupType.hpp"
#include "ResourceIdentifiers.hpp"

class Aircraft;

class Pickup : public Entity
{
public:
	Pickup(PickupType type, const TextureHolder& texture);
	virtual unsigned int GetCategory() const override;
	virtual sf::FloatRect GetBoundingRect() const;
	void Apply(Aircraft& player) const;
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

private:
	PickupType m_type;
	sf::Sprite m_sprite;
};

