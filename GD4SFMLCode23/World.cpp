#include "World.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Utility.hpp"
#include "ParticleNode.hpp"
#include "ParticleType.hpp"
#include "SoundNode.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <limits>

World::World(sf::RenderTarget& output_target, FontHolder& font, SoundPlayer& sounds, bool networked)
	:m_target(output_target)
	,m_camera(output_target.getDefaultView())
	,m_textures()
	,m_fonts(font)
	,m_sounds(sounds)
	,m_scenegraph()
	,m_scene_layers()
	,m_world_bounds(0.f, 0.f, m_camera.getSize().x, 5000.f)
	,m_spawn_position(m_camera.getSize().x/2.f, m_world_bounds.height - m_camera.getSize().y/2.f)
	,m_scrollspeed(-50.f)
	,m_scrollspeed_compensation(1.f)
	,m_player_aircraft()
	,m_enemy_spawn_points()
	,m_active_enemies()
	,m_networked_world(networked)
	,m_network_node(nullptr)
	,m_finish_sprite(nullptr)
{
	m_scene_texture.create(m_target.getSize().x, m_target.getSize().y);
	LoadTextures();
	BuildScene();

	m_camera.setCenter(m_spawn_position);
}

void World::SetWorldScrollCompensation(float compensation)
{
	m_scrollspeed_compensation = compensation;
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	m_camera.move(0, m_scrollspeed * dt.asSeconds()*m_scrollspeed_compensation);
	
	for (Aircraft* a : m_player_aircraft)
	{
		a->SetVelocity(0.f, 0.f);
	}

	DestroyEntitiesOutsideView();
	GuideMissiles();


	//Forward the commands to the scenegraph, sort out velocity
	while (!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}
	AdaptPlayerVelocity();

	HandleCollisions();

	//Remove all of the destroyed entities
	//RemoveWrecks() only destroys the entities, not the pointers in m_player_aircraft
	auto first_to_remove = std::remove_if(m_player_aircraft.begin(), m_player_aircraft.end(), std::mem_fn(&Aircraft::IsMarkedForRemoval));
	m_player_aircraft.erase(first_to_remove, m_player_aircraft.end());
	m_scenegraph.RemoveWrecks();

	SpawnEnemies();
	
	//Apply movement
	m_scenegraph.Update(dt, m_command_queue);
	AdaptPlayerPosition();

	UpdateSounds();
}

void World::Draw()
{
	if (PostEffect::IsSupported())
	{
		m_scene_texture.clear();
		m_scene_texture.setView(m_camera);
		m_scene_texture.draw(m_scenegraph);
		m_scene_texture.display();
		m_bloom_effect.Apply(m_scene_texture, m_target);
	}
	else
	{
		m_target.setView(m_camera);
		m_target.draw(m_scenegraph);
	}
}

Aircraft* World::GetAircraft(int identifier) const
{
	for (Aircraft* a : m_player_aircraft)
	{
		if (a->GetIdentifier() == identifier)
		{
			return a;
		}
	}
	return nullptr;
}

void World::RemoveAircraft(int identifier)
{
	Aircraft* aircraft = GetAircraft(identifier);
	if (aircraft)
	{
		aircraft->Destroy();
		m_player_aircraft.erase(std::find(m_player_aircraft.begin(), m_player_aircraft.end(), aircraft));
	}
}

Aircraft* World::AddAircraft(int identifier)
{
	std::unique_ptr<Aircraft> player(new Aircraft(AircraftType::kEagle, m_textures, m_fonts));
	player->setPosition(m_camera.getCenter());
	player->SetIdentifier(identifier);

	m_player_aircraft.emplace_back(player.get());
	m_scene_layers[static_cast<int>(Layers::kUpperAir)]->AttachChild(std::move(player));
	return m_player_aircraft.back();
}

void World::CreatePickup(sf::Vector2f position, PickupType type)
{
	std::unique_ptr<Pickup> pickup(new Pickup(type, m_textures));
	pickup->setPosition(position);
	pickup->SetVelocity(0.f, 1.f);
	m_scene_layers[static_cast<int>(Layers::kUpperAir)]->AttachChild(std::move(pickup));
}

bool World::PollGameAction(GameActions::Action& out)
{
	return m_network_node->PollGameAction(out);
}

void World::SetCurrentBattleFieldPosition(float lineY)
{
	m_camera.setCenter(m_camera.getCenter().x, lineY - m_camera.getSize().y / 2);
	m_spawn_position.y = m_world_bounds.height;
}

void World::SetWorldHeight(float height)
{
	m_world_bounds.height = height;
}

bool World::HasAlivePlayer() const
{
	return !m_player_aircraft.empty();
}

bool World::HasPlayerReachedEnd() const
{
	if (Aircraft* aircraft = GetAircraft(1))
	{
		return !m_world_bounds.contains(aircraft->getPosition());
	}
	return false;
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

void World::LoadTextures()
{
	m_textures.Load(Texture::kEntities, "Media/Textures/Entities.png");
	m_textures.Load(Texture::kJungle, "Media/Textures/Jungle.png");
	m_textures.Load(Texture::kExplosion, "Media/Textures/Explosion.png");
	m_textures.Load(Texture::kParticle, "Media/Textures/Particle.png");
	m_textures.Load(Texture::kFinishLine, "Media/Textures/FinishLine.png");
}

void World::BuildScene()
{
	//Initialize the different layers
	for(std::size_t i=0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{ 
		ReceiverCategories category = (i == static_cast<int>(Layers::kLowerAir)) ? ReceiverCategories::kScene : ReceiverCategories::kNone;
		SceneNode::Ptr layer(new SceneNode(category));
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = m_textures.Get(Texture::kJungle);
	texture.setRepeated(true);

	float view_height = m_camera.getSize().y;
	sf::IntRect texture_rect(m_world_bounds);
	texture_rect.height += static_cast<int>(view_height);

	//Add the background sprite to the world
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, texture_rect));
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top - view_height);
	m_scene_layers[static_cast<int>(Layers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add the finish line to the scene
	sf::Texture& finish_texture = m_textures.Get(Texture::kFinishLine);
	std::unique_ptr<SpriteNode> finish_sprite(new SpriteNode(finish_texture));
	finish_sprite->setPosition(0.f, -76.f);
	m_finish_sprite = finish_sprite.get();
	m_scene_layers[static_cast<int>(Layers::kBackground)]->AttachChild(std::move(finish_sprite));


	// Add particle node to the scene
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(ParticleType::kSmoke, m_textures));
	m_scene_layers[static_cast<int>(Layers::kLowerAir)]->AttachChild(std::move(smokeNode));

	// Add propellant particle node to the scene
	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(ParticleType::kPropellant, m_textures));
	m_scene_layers[static_cast<int>(Layers::kLowerAir)]->AttachChild(std::move(propellantNode));


	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(m_sounds));
	m_scenegraph.AttachChild(std::move(soundNode));

	if (m_networked_world)
	{
		std::unique_ptr<NetworkNode> network_node(new NetworkNode());
		m_network_node = network_node.get();
		m_scenegraph.AttachChild(std::move(network_node));
	}

	AddEnemies();
}

void World::AdaptPlayerPosition()
{
	//Keep the player on the sceen 
	sf::FloatRect view_bounds = GetViewBounds();
	const float border_distance = 40.f;
	
	for(Aircraft* aircraft : m_player_aircraft)
	{
		sf::Vector2f position = aircraft->getPosition();
		position.x = std::max(position.x, view_bounds.left + border_distance);
		position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
		position.y = std::max(position.y, view_bounds.top + border_distance);
		position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
		aircraft->setPosition(position);
	}
}

void World::AdaptPlayerVelocity()
{
	for (Aircraft* aircraft : m_player_aircraft)
	{
		sf::Vector2f velocity = aircraft->GetVelocity();

		//If moving diagonally, reduce velocity (to have always same velocity)
		if (velocity.x != 0.f && velocity.y != 0.f)
		{
			aircraft->SetVelocity(velocity / std::sqrt(2.f));
		}

		//Add scrolling velocity
		aircraft->Accelerate(0.f, m_scrollspeed);
	}
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
}

sf::FloatRect World::GetBattlefieldBounds() const
{
	//Return camera bounds + a small area at the top where enemies spawn offscreen
	sf::FloatRect bounds = GetViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::SpawnEnemies()
{
	//Spawn an enemy when it is relevant i.e when about to come on screen - in BattleFieldBounds
	while (!m_enemy_spawn_points.empty() && m_enemy_spawn_points.back().m_y > GetBattlefieldBounds().top)
	{
		SpawnPoint spawn = m_enemy_spawn_points.back();
		
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.m_type, m_textures, m_fonts));
		enemy->setPosition(spawn.m_x, spawn.m_y);
		enemy->setRotation(180.f);
		//If the game is networked the server is responsible for spawning pickups
		
		if (m_networked_world)
		{
			enemy->DisablePickups();
		}
		m_scene_layers[static_cast<int>(Layers::kUpperAir)]->AttachChild(std::move(enemy));
		m_enemy_spawn_points.pop_back();
	}
}

void World::AddEnemy(AircraftType type, float relX, float relY)
{
	SpawnPoint spawn(type, m_spawn_position.x + relX, m_spawn_position.y - relY);
	m_enemy_spawn_points.emplace_back(spawn);
}

void World::AddEnemies()
{
	if (m_networked_world)
	{
		return;
	}
	//Add all emenies
	AddEnemy(AircraftType::kRaptor, 0.f, 500.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 1000.f);
	AddEnemy(AircraftType::kRaptor, +100.f, 1150.f);
	AddEnemy(AircraftType::kRaptor, -100.f, 1150.f);
	AddEnemy(AircraftType::kAvenger, 70.f, 1500.f);
	AddEnemy(AircraftType::kAvenger, -70.f, 1500.f);
	AddEnemy(AircraftType::kAvenger, -70.f, 1710.f);
	AddEnemy(AircraftType::kAvenger, 70.f, 1700.f);
	AddEnemy(AircraftType::kAvenger, 30.f, 1850.f);
	AddEnemy(AircraftType::kRaptor, 300.f, 2200.f);
	AddEnemy(AircraftType::kRaptor, -300.f, 2200.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 2200.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 2500.f);
	AddEnemy(AircraftType::kAvenger, -300.f, 2700.f);
	AddEnemy(AircraftType::kAvenger, -300.f, 2700.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 3000.f);
	AddEnemy(AircraftType::kRaptor, 250.f, 3250.f);
	AddEnemy(AircraftType::kRaptor, -250.f, 3250.f);
	AddEnemy(AircraftType::kAvenger, 0.f, 3500.f);
	AddEnemy(AircraftType::kAvenger, 0.f, 3700.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 3800.f);
	AddEnemy(AircraftType::kAvenger, 0.f, 4000.f);
	AddEnemy(AircraftType::kAvenger, -200.f, 4200.f);
	AddEnemy(AircraftType::kRaptor, 200.f, 4200.f);
	AddEnemy(AircraftType::kRaptor, 0.f, 4400.f);

	//Sort according to y value so that lower enemies are checked first
	SortEnemies();
}

void World::SortEnemies()
{
	//Sort all enemies according to their y-value, such that lower enemies are checked first for spawning
	std::sort(m_enemy_spawn_points.begin(), m_enemy_spawn_points.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.m_y < rhs.m_y;
	});
}

void World::DestroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<int>(ReceiverCategories::kEnemyAircraft) | static_cast<int>(ReceiverCategories::kProjectile);
	command.action = DerivedAction<Entity>([this](Entity& e, sf::Time dt)
	{
		//Does the object interest with battlefield?
		if (!GetBattlefieldBounds().intersects(e.GetBoundingRect()))
		{
			e.Remove();
		}
	});
	m_command_queue.Push(command);
}

void World::GuideMissiles()
{
	//Target the closest enemy in the world
	Command enemyCollector;
	enemyCollector.category = static_cast<int>(ReceiverCategories::kEnemyAircraft);
	enemyCollector.action = DerivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
	{
		if (!enemy.IsDestroyed())
		{
			m_active_enemies.emplace_back(&enemy);
		}
	});

	Command missileGuider;
	missileGuider.category = static_cast<int>(ReceiverCategories::kAlliedProjectile);
	missileGuider.action = DerivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
	{
		if (!missile.IsGuided())
		{
			return;
		}

		float min_distance = std::numeric_limits<float>::max();
		Aircraft* closest_enemy = nullptr;

		for (Aircraft* enemy : m_active_enemies)
		{
			float enemy_distance = Distance(missile, *enemy);

			if (enemy_distance < min_distance)
			{
				closest_enemy = enemy;
				min_distance = enemy_distance;
			}
		}

		if (closest_enemy)
		{
			missile.GuideTowards(closest_enemy->GetWorldPosition());
		}
	});

	m_command_queue.Push(enemyCollector);
	m_command_queue.Push(missileGuider);
	m_active_enemies.clear();
}

bool MatchesCategories(SceneNode::Pair& colliders, ReceiverCategories type1, ReceiverCategories type2)
{
	unsigned int category1 = colliders.first->GetCategory();
	unsigned int category2 = colliders.second->GetCategory();
	if (static_cast<int>(type1) & category1 && static_cast<int>(type2) & category2)
	{
		return true;
	}
	else if (static_cast<int>(type1) & category2 && static_cast<int>(type2) & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::HandleCollisions()
{
	std::set<SceneNode::Pair> collision_pairs;
	m_scenegraph.CheckSceneCollision(m_scenegraph, collision_pairs);
	for (SceneNode::Pair pair : collision_pairs)
	{
		if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kEnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);
			//Collision Response
			player.Damage(enemy.GetHitPoints());
			enemy.Destroy();
		}
		else if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kPickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			//Collision Response
			pickup.Apply(player);
			pickup.Destroy();
			player.PlayLocalSound(m_command_queue, SoundEffect::kCollectPickup);
		}
		else if (MatchesCategories(pair, ReceiverCategories::kPlayerAircraft, ReceiverCategories::kEnemyProjectile) || MatchesCategories(pair, ReceiverCategories::kEnemyAircraft, ReceiverCategories::kAlliedProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			//Collision Response
			aircraft.Damage(projectile.GetDamage());
			projectile.Destroy();
		}
	}
}

void World::UpdateSounds()
{
	sf::Vector2f listener_position;

	// 0 players (multiplayer mode, until server is connected) -> view center
	if (m_player_aircraft.empty())
	{
		listener_position = m_camera.getCenter();
	}

	// 1 or more players -> mean position between all aircrafts
	else
	{
		for (Aircraft* aircraft : m_player_aircraft)
		{
			listener_position += aircraft->GetWorldPosition();
		}

		listener_position /= static_cast<float>(m_player_aircraft.size());
	}

	// Set listener's position
	m_sounds.SetListenerPosition(listener_position);

	// Remove unused sounds
	m_sounds.RemoveStoppedSounds();
}


