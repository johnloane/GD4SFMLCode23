#include "Aircraft.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "ResourceHolder.hpp"
#include "Texture.hpp"

Texture ToTextureID(AircraftType type)
{
	switch (type)
	{
	case AircraftType::kEagle:
		return Texture::kEagle;
		break;
	case AircraftType::kRaptor:
		return Texture::kRaptor;
		break;
	}
	return Texture::kEagle;
}

Aircraft::Aircraft(AircraftType type, const TextureHolder& textures) : m_type(type), m_sprite(textures.Get(ToTextureID(type)))
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
