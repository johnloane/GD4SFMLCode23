#pragma once
#include <map>
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include "Action.hpp"

class KeyBinding
{
public:
	explicit KeyBinding(int control_preconfiguration);

	void AssignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key GetAssignedKey(Action action) const;

	bool CheckAction(sf::Keyboard::Key key, Action& out) const;
	std::vector<Action>	GetRealtimeActions() const;

private:
	std::map<sf::Keyboard::Key, Action>	m_key_map;
};

bool IsRealtimeAction(Action action);

