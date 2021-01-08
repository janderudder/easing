#pragma once
#include <SFML/Graphics.hpp>
#include "sfext/Window.hpp"

class Mouse_controller
{
    sf::Vector2i m_previous_position;
    sf::Vector2i m_position;

public:
    void update(Window const& window)
    {
        m_previous_position = m_position;
        m_position = sf::Mouse::getPosition(window.render_window());
    }

     template <typename T =int>
    auto delta() const -> sf::Vector2<T>
    {
        return static_cast<sf::Vector2<T>>(m_previous_position - m_position);
    }
};
