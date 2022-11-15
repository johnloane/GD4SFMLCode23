#include "Label.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{

    Label::Label(const std::string& text, const FontHolder& fonts)
        : m_text(text, fonts.Get(Font::kMain), 16)
    {
    }

    bool Label::IsSelectable() const
    {
        return false;
    }

    void Label::SetText(const std::string& text)
    {
        m_text.setString(text);
    }

    void Label::HandleEvent(const sf::Event& event)
    {
    }

    void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(m_text, states);
    }
}
