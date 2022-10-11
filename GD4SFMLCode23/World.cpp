#include "World.hpp"

World::World(sf::RenderWindow& window)
	:m_window(window)
	,m_camera(window.getDefaultView())
	,m_textures()
	,m_scenegraph()
	,m_scene_layers()
	,m_world_bounds(0.f, 0.f, m_camera.getSize().x, 2000.f)
	,m_spawn_position(m_camera.getSize().x/2.f, m_world_bounds.height - m_camera.getSize().y/2.f)
	,m_scrollspeed(-50.f)
	,m_player_aircraft(nullptr)
{
	LoadTextures();
	BuildScene();

	m_camera.setCenter(m_spawn_position);
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	m_camera.move(0, m_scrollspeed * dt.asSeconds());
	sf::Vector2f position = m_player_aircraft->getPosition();
	sf::Vector2f velocity = m_player_aircraft->GetVelocity();

	//If the player touches the x-boundaries flip their x-velocity
	if (position.x < m_world_bounds.left + 150.f || position.x > m_world_bounds.left + m_world_bounds.width - 150.f)
	{
		velocity.x = -velocity.x;
		m_player_aircraft->SetVelocity(velocity);
	}

	m_scenegraph.Update(dt);
}

void World::Draw()
{
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

void World::LoadTextures()
{
	m_textures.Load(Texture::kEagle, "Media/Textures/Eagle.png");
	m_textures.Load(Texture::kRaptor, "Media/Textures/Raptor.png");
	m_textures.Load(Texture::kDesert, "Media/Textures/Desert.png");

}

void World::BuildScene()
{
}
