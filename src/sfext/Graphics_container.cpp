#include "sfext/Graphics_container.hpp"



// visitor for getting a sf::Transform from a transform_sumtype
////////////////////////////////////////////////////////////////////////////////
namespace
{
constexpr auto transform_sumtype_visitor = [](auto&& one_of_sumtype) noexcept
{
    using ref_t = std::reference_wrapper<sf::Transform>;
    using cref_t = std::reference_wrapper<sf::Transform const>;

    using T = std::decay_t<decltype(one_of_sumtype)>;

    if constexpr (std::is_invocable_v<T>)
    {
        return std::invoke(one_of_sumtype);
    }
    else if constexpr (std::is_same_v<T, sf::Transform>)
    {
        return one_of_sumtype;
    }
    else
    {
        return one_of_sumtype.get();
    }
};
} // ::



// nested struct: value_type
////////////////////////////////////////////////////////////////////////////////
Graphics_container::value_type::value_type(
    sf::Drawable const& drawable,
    transform_sumtype   transform_variant
) noexcept
    : nm_drawable_ref       {std::cref(drawable)}
    , nm_transform_variant  {std::move(transform_variant)}
{
}



// ctor
////////////////////////////////////////////////////////////////////////////////
Graphics_container::Graphics_container(std::initializer_list<value_type> values)
 noexcept
    : m_values {values}
{
}



// mutate
////////////////////////////////////////////////////////////////////////////////
void Graphics_container::add(
    sf::Drawable const& drawable,
    transform_sumtype   transform_variant
){
    m_values.push_back({drawable, std::move(transform_variant)});
}



// void Graphics_container::add(std::initializer_list<value_type> values)
// {
//     m_values.insert(m_values.end(), values);
// }



// info
////////////////////////////////////////////////////////////////////////////////
auto Graphics_container::empty() const -> bool
{
    return m_values.empty();
}



auto Graphics_container::size() const -> size_t
{
    return m_values.size();
}



// private
////////////////////////////////////////////////////////////////////////////////
void Graphics_container::draw(sf::RenderTarget& target, sf::RenderStates states)
 const
 {
    states.transform *= getTransform();

    for (auto const& [drawable_ref, transform_variant]: std::as_const(m_values))
    {
        target.draw(
            drawable_ref.get(),
            sf::RenderStates{
                states.blendMode,
                states.transform
                    * std::visit(transform_sumtype_visitor, transform_variant),
                states.texture,
                states.shader
            }
        );
    }
}
