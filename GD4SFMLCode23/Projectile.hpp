#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "ProjectileType.hpp"
#include "ResourceIdentifiers.hpp"

class Projectile : public Entity
{
public:
	Projectile(ProjectileType type, const TextureHolder& textures);
	void GuideTowards(sf::Vector2f position);
	bool IsGuided() const;

	unsigned int GetCategory() const override;
	sf::FloatRect GetBoundingRect() const override;
	float GetMaxSpeed() const;
	int GetDamage() const;

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	ProjectileType m_type;
	sf::Sprite m_sprite;
	sf::Vector2f m_target_direction;
};

