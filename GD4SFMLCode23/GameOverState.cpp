#include "GameOverState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Player.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

GameOverState::GameOverState(StateStack& stack, Context context, const std::string& text)
    : State(stack, context)
    , m_game_over_text()
    , m_elapsed_time(sf::Time::Zero)
{
    sf::Font& font = context.fonts->Get(Font::kMain);
    sf::Vector2f window_size(context.window->getSize());

    m_game_over_text.setFont(font);
	m_game_over_text.setString(text);

    m_game_over_text.setCharacterSize(70);
    Utility::CentreOrigin(m_game_over_text);
    m_game_over_text.setPosition(0.5f * window_size.x, 0.4 * window_size.y);
}

void GameOverState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());

    //Create dark semi-transparent background
    sf::RectangleShape background_shape;
    background_shape.setFillColor(sf::Color(0, 0, 0, 150));
    background_shape.setSize(window.getView().getSize());

    window.draw(background_shape);
    window.draw(m_game_over_text);
}

bool GameOverState::Update(sf::Time dt)
{
    //Show state for 3 seconds and then return to the menu
    m_elapsed_time += dt;
    if (m_elapsed_time > sf::seconds(3))
    {
        RequestStackClear();
        RequestStackPush(StateID::kMenu);
    }
    return false;
}

bool GameOverState::HandleEvent(const sf::Event& event)
{
    return false;
}
