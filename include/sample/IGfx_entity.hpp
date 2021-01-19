#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>


struct IGfx_entity
    : virtual public sf::Drawable
    , virtual public sf::Transformable
{
};
