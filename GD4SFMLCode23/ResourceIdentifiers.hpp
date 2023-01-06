#pragma once
#include "Texture.hpp"
#include "Font.hpp"
#include "ShaderTypes.hpp"


namespace sf
{
	class Texture;
	class Font;
	class Shader;
}

template<typename Resouce, typename Identifier>
class ResourceHolder;

typedef ResourceHolder < sf::Texture, Texture> TextureHolder;
typedef ResourceHolder < sf::Font, Font> FontHolder;
typedef ResourceHolder<sf::Shader, ShaderTypes> ShaderHolder;