#pragma once
#include "ReceiverCategories.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include <cassert>
#include "SceneNode.hpp"

class SceneNode;

struct Command
{
	Command();
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

