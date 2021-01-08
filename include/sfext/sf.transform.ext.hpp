#pragma once
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>

namespace sfx
{
sf::Vector2f get_translation(sf::Transform const& transform);

void reset_translation(sf::Transform& transform);
void reset_rotation(sf::Transform& transform);
void reset_scale(sf::Transform& transform);
void output_matrix(sf::Transform const&);

sf::Transform make_translated_transform(sf::Vector2f const&) noexcept;
sf::Transform make_translated_transform(float, float) noexcept;

sf::Transform null_transform() noexcept;
sf::Transform null_transform_if(bool) noexcept;


} // ::sfx



// impl.
////////////////////////////////////////////////////////////////////////////////
namespace sfx
{

inline sf::Vector2f get_translation(sf::Transform const& transform)
{
    return transform.transformPoint({0,0});
}




inline void reset_translation(sf::Transform& transform)
{
    transform.translate(
        transform.getInverse()
            .transformPoint({0,0})
    );
}




inline void reset_rotation(sf::Transform& transform)
{
    transform.rotate(
        transform.getInverse()
            .transformPoint({0, 0}).x
    );
}




inline void reset_scale(sf::Transform& transform)
{
    transform.scale(
        transform.getInverse()
            .transformPoint({0, 0})
    );
}




inline void output_matrix(sf::Transform const& transform)
{
    auto const matrix = transform.getMatrix();
    std::array<std::array<float, 4>, 4> lines;

    for (int j=0; j < 4; ++j)
        for (int i=0; i < 4; ++i)
            lines[j][i] = matrix[i*4+j];

    std::cout.precision(2);

    for (auto const& line : std::as_const(lines))
    {
        std::cout << "[ ";

        for (auto const& n : std::as_const(line))
            std::cout << std::setw(10) << n << " ";

        std::cout << " ]\n";
    }

    std::cout.precision(0ULL);
}




inline sf::Transform make_translated_transform(sf::Vector2f const& translation)noexcept
{
    return sf::Transform{}.translate(translation);
}




inline sf::Transform make_translated_transform(float x, float y) noexcept
{
    return sf::Transform{}.translate(x, y);
}




inline sf::Transform null_transform() noexcept
{
    return sf::Transform{}.scale(0.f, 0.f);
}




inline sf::Transform null_transform_if(bool _value) noexcept
{
    return (_value)
        ? null_transform()
        : sf::Transform{};
}



} // namespace sfx
