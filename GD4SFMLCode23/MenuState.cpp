#include "MenuState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

#include "ResourceHolder.hpp"
#include "Utility.hpp"

MenuState::MenuState(StateStack& stack, Context context)
    :State(stack, context)
    , m_option_index(0)
{
    sf::Texture& texture = context.textures->Get(Texture::kTitleScreen);
    sf::Font& font = context.fonts->Get(Font::Main);

    m_background_sprite.setTexture(texture);

    //A simple menu
    sf::Text play_option;
    play_option.setFont(font);
    play_option.setString("Play");
    Utility::CentreOrigin(play_option);
    play_option.setPosition(context.window->getView().getSize() / 2.f);
    m_options.emplace_back(play_option);

    sf::Text exit_option;
    exit_option.setFont(font);
    exit_option.setString("Exit");
    Utility::CentreOrigin(exit_option);
    exit_option.setPosition(play_option.getPosition() + sf::Vector2f(0.f, 30.f));
    m_options.emplace_back(exit_option);

    UpdateOptionText();
}

void MenuState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());
    window.draw(m_background_sprite);
    for (const sf::Text& text : m_options)
    {
        window.draw(text);
    }
}

bool MenuState::Update()
{
    return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed)
    {
        return false;
    }

    if (event.key.code == sf::Keyboard::Return)
    {
        if (m_option_index == static_cast<int>(MenuOptions::kPlay))
        {
            RequestStackPop();
            RequestStackPush(StateID::kGame);
        }
         
        else if (m_option_index == static_cast<int>(MenuOptions::kExit))
        {
            RequestStackPop();
        }
    }

    else if (event.key.code == sf::Keyboard::Up)
    {
        if (m_option_index > 0)
        {
            m_option_index--;
        }
        else
        {
            m_option_index = m_options.size() - 1;
        }
        UpdateOptionText();
    }

    else if (event.key.code == sf::Keyboard::Down)
    {
        if (m_option_index < m_options.size() - 1)
        {
            m_option_index++;
        }
        else
        {
            m_option_index = 0;
        }
        UpdateOptionText();
    }
    return true;
}

void MenuState::UpdateOptionText()
{
    if (m_options.empty())
    {
        return;
    }
    for (sf::Text& text : m_options)
    {
        text.setFillColor(sf::Color::White);
    }
    m_options[m_option_index].setFillColor(sf::Color::Red);
}
