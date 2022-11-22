#pragma once
#include <string>
#include <SFML/Window/Keyboard.hpp>
namespace sf
{
	class Sprite;
	class Text;
}

class Utility
{
public:
	static void CentreOrigin(sf::Sprite& sprite);
	static void CentreOrigin(sf::Text& text);
	static std::string toString(sf::Keyboard::Key key);
};

