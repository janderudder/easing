#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <functional>
#include <initializer_list>
#include <variant>
#include <vector>


class Graphics_container
    : public sf::Drawable
    , public sf::Transformable
{
public:
    using transform_sumtype = std::variant <
        sf::Transform,
        std::reference_wrapper<sf::Transform>,
        std::reference_wrapper<sf::Transform const>,
        std::function<sf::Transform()>
    >;

private:
    struct value_type
    {
        std::reference_wrapper<sf::Drawable const>  entity;
        transform_sumtype                           transform_variant;

        value_type(sf::Drawable const&, transform_sumtype ={}) noexcept;
    };

    std::vector<value_type> m_values;

public:
    Graphics_container() = default;
    Graphics_container(std::initializer_list<value_type>) noexcept;

    void add(sf::Drawable const&, transform_sumtype = {});
    void add(std::initializer_list<value_type>);

    auto empty() const -> bool;
    auto size() const -> size_t;

private:
    void draw(sf::RenderTarget&, sf::RenderStates= sf::RenderStates::Default)
        const override;

};
