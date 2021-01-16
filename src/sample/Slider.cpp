#include "sample/Slider.hpp"



Slider::Slider(float length, float radius, float ratio) noexcept
    : m_circle  {radius}
    , m_line    {{length, 2.f}}
{
    m_line.setFillColor(sf::Color::Black);

    m_circle.setFillColor({255, 255, 255, 186});
    m_circle.setOutlineColor({0, 0, 0, 255});
    m_circle.setOutlineThickness(2.5f);
    m_circle.setOrigin(radius, radius);

    set_ratio(_validate_ratio(ratio));
}


////////////////////////////////////////////////////////////////////////////////
void Slider::set_ratio(float r)
{
    set_circle_position(_validate_ratio(r) * length());
}



void Slider::set_circle_position(float x)
{
    m_circle.setPosition(_validate_position(x), 0.f);
}



float Slider::ratio() const
{
    return length() / circle_position();
}



float Slider::circle_position() const
{
    return m_circle.getPosition().x;
}


float Slider::length() const
{
    return m_line.getSize().x;
}



////////////////////////////////////////////////////////////////////////////////
void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    target.draw(m_line, states);
    target.draw(m_circle, states);
}



float Slider::_validate_position(float x) const
{
    if (x < 0.f) {
        return 0.f;
    }
    else if (x > m_line.getSize().x) {
        return m_line.getSize().x;
    }
    else {
        return x;
    }
}



float Slider::_validate_ratio(float r) const
{
    if (r < 0.f) {
        return 0.f;
    }
    else if (r > 1.f) {
        return 1.f;
    }
    else {
        return r;
    }
}
