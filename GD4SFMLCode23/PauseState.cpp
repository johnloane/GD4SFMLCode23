#include "PauseState.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Utility.hpp"
#include "Button.hpp"

PauseState::PauseState(StateStack& stack, Context context, bool lets_updates_through)
    : State(stack, context)
	, m_lets_updates_through(lets_updates_through)
{
    sf::Font& font = context.fonts->Get(Font::kMain);
    sf::Vector2f window_size(context.window->getSize());

    m_paused_text.setFont(font);
    m_paused_text.setString("Game Paused");
    m_paused_text.setCharacterSize(70);
    Utility::CentreOrigin(m_paused_text);
    m_paused_text.setPosition(0.5f * window_size.x, 0.4f * window_size.y);

    auto returnButton = std::make_shared<GUI::Button>(context);
    returnButton->setPosition(0.5f * window_size.x - 100, 0.4f * window_size.y + 75);
    returnButton->SetText("Return");
    returnButton->SetCallback([this]()
    {
        RequestStackPop();
    });

    auto backToMenuButton = std::make_shared<GUI::Button>(context);
    backToMenuButton->setPosition(0.5f * window_size.x - 100, 0.4f * window_size.y + 125);
    backToMenuButton->SetText("Back to menu");
    backToMenuButton->SetCallback([this]()
    {
        RequestStackClear();
    RequestStackPush(StateID::kMenu);
    });

    m_gui_container.Pack(returnButton);
    m_gui_container.Pack(backToMenuButton);

    context.music->SetPaused(true);
}

PauseState::~PauseState()
{
    GetContext().music->SetPaused(false);
}

void PauseState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(m_paused_text);
    window.draw(m_gui_container);
}

bool PauseState::Update(sf::Time dt)
{
    return m_lets_updates_through;
}

bool PauseState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
    return false;
}
