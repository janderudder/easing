#pragma once
#include <SFML/Graphics.hpp>
#include <array>

/*
    Slider's ratio is in interval [0, 1]
*/
class Slider
    : public sf::Drawable
    , public sf::Transformable
{
    sf::CircleShape     m_circle;
    sf::RectangleShape  m_line;

public:
    Slider(unsigned length, float radius = 16.f, float ratio = 0) noexcept;

    void set_ratio(float);
    void set_circle_position(float);
    void move_circle(float);

    float ratio() const;
    float circle_position() const;
    float length() const;

private:
    void draw(
        sf::RenderTarget&,
        sf::RenderStates = sf::RenderStates::Default
    ) const override;

    float _validate_position(float x) const;
    float _validate_ratio(float val) const;

};
