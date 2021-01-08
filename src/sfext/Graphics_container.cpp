#include "sfext/Graphics_container.hpp"



// visitor for getting a sf::Transform from a transform_sumtype
////////////////////////////////////////////////////////////////////////////////
namespace
{
    constexpr auto transform_sumtype_visitor = [](auto&& one_of_sumtype)
    {
using T = std::decay_t<decltype(one_of_sumtype)>;

if constexpr (std::is_same_v<T, sf::Transform>)
{
    return one_of_sumtype;
}
else if constexpr (std::is_same_v<T, std::reference_wrapper<sf::Transform>>)
{
    return one_of_sumtype.get();
}
else if constexpr (
    std::is_same_v<T, std::reference_wrapper<sf::Transform const>>)
{
    return one_of_sumtype.get();
}
else
{
    return std::invoke(one_of_sumtype);
}

    };
} // ::




// nested struct: value_type
////////////////////////////////////////////////////////////////////////////////
Graphics_container::value_type::value_type(
    sf::Drawable const& _entity,
    transform_sumtype   _transform_variant
) noexcept
    : entity            {std::cref(_entity)}
    , transform_variant {std::move(_transform_variant)}
{
}




// ctor
////////////////////////////////////////////////////////////////////////////////
Graphics_container::Graphics_container(std::initializer_list<value_type>entries)
 noexcept
    : m_values {entries}
{
}




// mutate
////////////////////////////////////////////////////////////////////////////////
void Graphics_container::add(
    sf::Drawable const& entity,
    transform_sumtype   transform_variant
)
{
    m_values.push_back({entity, std::move(transform_variant)});
}




void Graphics_container::add(std::initializer_list<value_type> entries)
{
    m_values.insert(m_values.end(), entries);
}




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
    auto const& container_trans = this->getTransform();

    for (auto const& [entity, transform_variant] : std::as_const(m_values))
    {
        auto entity_states = states;
        entity_states.transform *= container_trans;

        entity_states.transform
            *= std::visit(transform_sumtype_visitor, transform_variant);

        target.draw(entity.get(), entity_states);
    }
}
