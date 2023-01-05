#pragma once
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include "Animation.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
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
	static void CentreOrigin(Animation& animation);
	static std::string toString(sf::Keyboard::Key key);
	static double ToRadians(int degrees);
	static double ToDegrees(double angle);
	static sf::Vector2f UnitVector(sf::Vector2f vector);
	static float Length(sf::Vector2f vector);
	static int RandomInt(int exclusive_max);
};

