#include "Aircraft.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "ResourceHolder.hpp"
#include "ReceiverCategories.hpp"
#include "Texture.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ProjectileType.hpp"
#include "Projectile.hpp"

namespace
{
	const std::vector<AircraftData> Table = InitializeAircraftData();
}

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

Aircraft::Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts) 
	: Entity(Table[static_cast<int>(type)].m_hitpoints)
	, m_type(type) 
	, m_sprite(textures.Get(ToTextureID(type)))
	, m_is_firing(false)
	, m_is_launching_missile(false)
	, m_fire_countdown(sf::Time::Zero)
	, m_fire_rate(1)
	, m_spread_level(1)
	, m_missile_ammo(2)
	, m_health_display(nullptr)
	, m_missile_display(nullptr)
	, m_travelled_distance(0.f)
	, m_directions_index(0)
{
	m_fire_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_fire_command.action = [this, &textures](SceneNode& node, sf::Time dt)
	{
		CreateBullet(node, textures);
	};

	m_missile_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_missile_command.action = [this, &textures](SceneNode& node, sf::Time dt)
	{
		CreateProjectile(node, ProjectileType::kMissile, 0.f, 0.5f, textures);
	};


	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	std::string empty_string = "";

	std::unique_ptr<TextNode> health_display(new TextNode(fonts, empty_string));
	m_health_display = health_display.get();
	AttachChild(std::move(health_display));

	if (GetCategory() == static_cast<int>(ReceiverCategories::kPlayerAircraft))
	{
		std::unique_ptr<TextNode> missile_display(new TextNode(fonts, empty_string));
		missile_display->setPosition(0, 70);
		m_missile_display = missile_display.get();
		AttachChild(std::move(missile_display));
	}
	UpdateTexts();

}

unsigned int Aircraft::GetCategory() const
{
	switch (m_type)
	{
	case AircraftType::kEagle:
		return static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
	default:
		return static_cast<unsigned int>(ReceiverCategories::kEnemyAircraft);

	}
}

void Aircraft::IncreaseFireRate()
{
	if (m_fire_rate < 5)
	{
		++m_fire_rate;
	}
}

void Aircraft::IncreaseFireSpread()
{
	if (m_spread_level < 3)
	{
		++m_spread_level;
	}
}

void Aircraft::CollectMissiles(unsigned int count)
{
	m_missile_ammo += count;
}

void Aircraft::UpdateTexts()
{
	m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());

	if (m_missile_display)
	{
		if (m_missile_ammo == 0)
		{
			m_missile_display->SetString("");
		}
		else
		{
			m_missile_display->SetString("M: " + std::to_string(m_missile_ammo));
		}
	}
}

void Aircraft::UpdateMovementPattern(sf::Time dt)
{
	//Enemy AI
	const std::vector<Direction>& directions = Table[static_cast<int>(m_type)].m_directions;
	if (!directions.empty())
	{
		//Move along the current direction, then change direction
		if (m_travelled_distance > directions[m_directions_index].m_distance)
		{
			m_directions_index = (m_directions_index + 1) % directions.size();
			m_travelled_distance = 0;
		}

		//Compute velocity
		double radians = Utility::ToRadians(directions[m_directions_index].m_angle + 90.f);
		float vx = GetMaxSpeed() * std::cos(radians);
		float vy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(vx, vy);
		m_travelled_distance += GetMaxSpeed() * dt.asSeconds();



	}
}

float Aircraft::GetMaxSpeed() const
{
	return Table[static_cast<int>(m_type)].m_speed;
}

void Aircraft::Fire()
{
	if (Table[static_cast<int>(m_type)].m_fire_interval != sf::Time::Zero)
	{
		m_is_firing = true;
	}
}

void Aircraft::LaunchMissile()
{
	if (m_missile_ammo > 0)
	{
		m_is_launching_missile = true;
		--m_missile_ammo;
	}
}

void Aircraft::CreateBullet(SceneNode& node, const TextureHolder& textures) const
{
	ProjectileType type = IsAllied() ? ProjectileType::kAlliedBullet : ProjectileType::kEnemyBullet;
	switch (m_spread_level)
	{
	case 1:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	case 2:
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	case 3:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	}
}


void Aircraft::CreateProjectile(SceneNode& node, ProjectileType type, float x_offset, float y_offset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(x_offset * m_sprite.getGlobalBounds().width, y_offset * m_sprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->GetMaxSpeed());

	float sign = IsAllied() ? -1.f: +1.f;
	projectile->setPosition(GetWorldPosition() + offset * sign);
	projectile->SetVelocity(velocity * sign);
	node.AttachChild(std::move(projectile));
}

sf::FloatRect Aircraft::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Aircraft::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	UpdateTexts();
	Entity::UpdateCurrent(dt, commands);
	UpdateMovementPattern(dt);
}

void Aircraft::CheckProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	//Enemies try to fire as often as they can
	if (!IsAllied())
	{
		Fire();
	}

	if (m_is_firing && m_fire_countdown <= sf::Time::Zero)
	{
		commands.Push(m_fire_command);
		m_fire_countdown += Table[static_cast<int>(m_type)].m_fire_interval / (m_fire_rate + 1.f);
		m_is_firing = false;
	}
	else if (m_fire_countdown > sf::Time::Zero)
	{
		//Wait, can't fire
		m_fire_countdown -= dt;
		m_is_firing = false;
	}

	//Missile launch
	if (m_is_launching_missile)
	{
		commands.Push(m_missile_command);
		m_is_launching_missile = false;
	}

}

bool Aircraft::IsAllied() const
{
	return m_type == AircraftType::kEagle;
}
