#pragma once
#include "State.hpp"
#include "MenuOptions.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);
	virtual void Draw();
	virtual bool Update();
	virtual bool HandleEvent(const sf::Event& event);
	void UpdateOptionText();

private:
	sf::Sprite m_background_sprite;
	std::vector <sf::Text> m_options;
	std::size_t m_option_index;
};

