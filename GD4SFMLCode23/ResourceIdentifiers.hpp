#pragma once
#include "Texture.hpp"
#include "Font.hpp"


namespace sf
{
	class Texture;
	class Font;
}

template<typename Resouce, typename Identifier>
class ResourceHolder;

typedef ResourceHolder < sf::Texture, Texture> TextureHolder;
typedef ResourceHolder < sf::Font, Font> FontHolder;