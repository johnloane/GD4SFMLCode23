#pragma once
#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>

class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);
	virtual void Draw();
	virtual bool Update(sf::Time dt);
	virtual bool HandleEvent(const sf::Event& event);

private:
	void UpdateLabels();
	void AddButtonLabel(Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite m_background_sprite;
	GUI::Container m_gui_container;
	std::array<GUI::Button::Ptr, static_cast<int>(Action::kActionCount)> m_binding_buttons;
	std::array<GUI::Label::Ptr, static_cast<int>(Action::kActionCount)> m_binding_labels;

};

