#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "Layers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


#include <array>
#include "CommandQueue.hpp"



class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window, FontHolder& font);
	void Update(sf::Time dt);
	void Draw();
	CommandQueue& GetCommandQueue();

private:
	void LoadTextures();
	void BuildScene();
	void AdaptPlayerPosition();
	void AdaptPlayerVelocity();

private:
	sf::RenderWindow& m_window;
	sf::View m_camera;
	TextureHolder m_textures;
	FontHolder& m_fonts;
	SceneNode m_scenegraph;
	std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)> m_scene_layers;

	CommandQueue m_command_queue;

	sf::FloatRect m_world_bounds;
	sf::Vector2f m_spawn_position;
	float m_scrollspeed;
	Aircraft* m_player_aircraft;
};

