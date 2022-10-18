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
	
	m_player_aircraft->SetVelocity(0.f, 0.f);

	//Forward the commands to the scenegraph, sort out velocity
	while (!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}
	AdaptPlayerVelocity();

	m_scenegraph.Update(dt);
	AdaptPlayerPosition();
}

void World::Draw()
{
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

void World::LoadTextures()
{
	m_textures.Load(Texture::kEagle, "Media/Textures/Eagle.png");
	m_textures.Load(Texture::kRaptor, "Media/Textures/Raptor.png");
	m_textures.Load(Texture::kDesert, "Media/Textures/Desert.png");
}

void World::BuildScene()
{
	//Initialize the different layers
	for(std::size_t i=0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{ 
		SceneNode::Ptr layer(new SceneNode());
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = m_textures.Get(Texture::kDesert);
	sf::IntRect textureRect(m_world_bounds);
	texture.setRepeated(true);

	//Add the background sprite to the world
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, textureRect));
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::kEagle, m_textures));
	m_player_aircraft = leader.get();
	m_player_aircraft->setPosition(m_spawn_position);
	m_player_aircraft->SetVelocity(40.f, m_scrollspeed);

	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(leader));

	std::unique_ptr<Aircraft> left_escort(new Aircraft(AircraftType::kRaptor, m_textures));
	left_escort->setPosition(-80.f, 50.f);
	m_player_aircraft->AttachChild(std::move(left_escort));

	std::unique_ptr<Aircraft> right_escort(new Aircraft(AircraftType::kRaptor, m_textures));
	right_escort->setPosition(80.f, 50.f);
	m_player_aircraft->AttachChild(std::move(right_escort));


}

void World::AdaptPlayerPosition()
{
	//Keep the player on the sceen 
	sf::FloatRect view_bounds(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
	const float border_distance = 40.f;

	sf::Vector2f position = m_player_aircraft->getPosition();
	position.x = std::max(position.x, view_bounds.left + border_distance);
	position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
	position.y = std::max(position.y, view_bounds.top + border_distance);
	position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
	m_player_aircraft->setPosition(position);
}

void World::AdaptPlayerVelocity()
{
	sf::Vector2f velocity = m_player_aircraft->GetVelocity();

	//If they are moving diagonally divide by root 2
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		m_player_aircraft->SetVelocity(velocity / std::sqrt(2.f));
	}

	//Add scrolling velocity
	m_player_aircraft->Accelerate(0.f, m_scrollspeed);
}
