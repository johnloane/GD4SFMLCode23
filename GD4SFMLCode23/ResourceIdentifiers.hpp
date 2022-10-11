#pragma once
#include "Texture.hpp"

namespace sf
{
	class Texture;
}

template<typename Resouce, typename Identifier>
class ResourceHolder;

typedef ResourceHolder < sf::Texture, Texture> TextureHolder;