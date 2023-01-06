#pragma once
#include "PostEffect.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>


class BloomEffect : public PostEffect
{
public:
	BloomEffect();

	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);


private:
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;


private:
	void PrepareTextures(sf::Vector2u size);

	void FilterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void BlurMultipass(RenderTextureArray& renderTextures);
	void Blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void Downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void Add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);


private:
	ShaderHolder		m_shaders;

	sf::RenderTexture	m_brightness_texture;
	RenderTextureArray	m_firstpass_textures;
	RenderTextureArray	m_secondpass_textures;
};

