#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Texture.hpp"

class TextureHolder
{
public:
	void Load(Texture id, const std::string& filename);
	sf::Texture& Get(Texture id);
	const sf::Texture& Get(Texture id) const;
private:
	std::map<Texture, std::unique_ptr<sf::Texture>> m_texture_map;
};

