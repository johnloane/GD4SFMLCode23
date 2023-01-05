#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Particle.hpp"
#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"
#include "ParticleType.hpp"

class ParticleNode : public SceneNode
{
public:
	ParticleNode(ParticleType type, const TextureHolder& textures);

	void AddParticle(sf::Vector2f position);
	ParticleType GetParticleType() const;
	virtual unsigned int GetCategory() const;


private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	void AddVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
	void ComputeVertices() const;


private:
	std::deque<Particle> m_particles;
	const sf::Texture& m_texture;
	ParticleType m_type;

	mutable sf::VertexArray	m_vertex_array;
	mutable bool m_needs_vertex_update;
};

