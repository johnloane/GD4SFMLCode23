#include "Button.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{

    Button::Button(const FontHolder& fonts, const TextureHolder& textures)
        :m_normal_texture(textures.Get(Texture::kButtonNormal))
        , m_selected_texture(textures.Get(Texture::kButtonSelected))
        , m_pressed_texture(textures.Get(Texture::kButtonPressed))
        , m_text("", fonts.Get(Font::kMain), 16)
        , m_is_toggle(false)
    {
        m_sprite.setTexture(m_normal_texture);
        sf::FloatRect bounds = m_sprite.getLocalBounds();
        m_text.setPosition(bounds.width / 2, bounds.height / 2);
    }

    void Button::SetCallback(Callback callback)
    {
        m_callback = std::move(callback);
    }

    void Button::SetText(const std::string& text)
    {
        m_text.setString(text);
        Utility::CentreOrigin(m_text);
    }

    void Button::SetToggle(bool flag)
    {
        m_is_toggle = flag;
    }

    bool Button::IsSelectable() const
    {
        return true;
    }

    void Button::Select()
    {
        Component::Select();
        m_sprite.setTexture(m_selected_texture);
    }

    void Button::Deselect()
    {
        Component::Deselect();
        m_sprite.setTexture(m_normal_texture);
    }

    void Button::Activate()
    {
        Component::Activate();
        //If toggle then show button is pressed or toggled
        if (m_is_toggle)
        {
            m_sprite.setTexture(m_pressed_texture);
        }
        if (m_callback)
        {
            m_callback();
        }
        if (!m_is_toggle)
        {
            Deactivate();
        }
    }

    void Button::Deactivate()
    {
        Component::Deactivate();
        if (m_is_toggle)
        {
            if (IsSelected())
            {
                m_sprite.setTexture(m_selected_texture);
            }
            else
            {
                m_sprite.setTexture(m_normal_texture);
            }
        }
    }

    void Button::HandleEvent(const sf::Event& event)
    {
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
        target.draw(m_text, states);
    }
}