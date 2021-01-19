#include "sample/Colored_square.hpp"



Colored_square::Colored_square(sf::Color start_color, sf::Vector2f size)
 noexcept
    : m_rect {size}
{
    m_rect.setFillColor(start_color);
    m_rect.setOutlineColor(sf::Color::Black);
    m_rect.setOutlineThickness(2.f);
}



////////////////////////////////////////////////////////////////////////////////
void Colored_square::set_color(sf::Color color)
{
    m_rect.setFillColor(color);
}



auto Colored_square::color() const -> sf::Color
{
    return m_rect.getFillColor();
}



////////////////////////////////////////////////////////////////////////////////
void Colored_square::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    target.draw(m_rect, states);
}
