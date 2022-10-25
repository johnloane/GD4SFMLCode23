#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "State.hpp"

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);
	virtual void Draw();
	virtual bool Update(sf::Time dt);
	virtual bool HandleEvent(const sf::Event& event);

private:
	sf::Sprite m_background_sprite;

};

