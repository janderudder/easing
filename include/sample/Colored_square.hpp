#pragma once
#include "sample/IGfx_entity.hpp"
#include <SFML/Graphics.hpp>


class Colored_square: public IGfx_entity
{
    sf::RectangleShape m_rect;

public:
    Colored_square(sf::Color start_color, sf::Vector2f size ={64,64}) noexcept;

    void set_color(sf::Color);
    auto color() const -> sf::Color;

private:
    void draw(
        sf::RenderTarget&,
        sf::RenderStates = sf::RenderStates::Default
    ) const override;

};
