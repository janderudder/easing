#include "sfext/Grid_layout_simple.hpp"
#include <cmath>


// nested class
////////////////////////////////////////////////////////////////////////////////
Grid_layout_simple::assignable_cell_t::assignable_cell_t(
      Grid_layout_simple*   parent_ptr
    , std::size_t           idx
) noexcept
    : parent {parent_ptr}
    , index  {idx}
{
}




Grid_layout_simple& Grid_layout_simple::assignable_cell_t::operator=(
    sf::Drawable const& drawable
) noexcept
{
    this->parent->m_drawables[this->index] = &drawable;
    return *parent;
}




Grid_layout_simple::assignable_cell_t::operator sf::Drawable const&() const
{
    return *(this->parent->m_drawables[this->index]);
}




// Grid_layout_simple constructor
////////////////////////////////////////////////////////////////////////////////
Grid_layout_simple::Grid_layout_simple(
    unsigned     column_count,
    sf::Vector2f cell_size
) noexcept
    : m_column_count {column_count}
    , m_cell_size    {std::move(cell_size)}
{
}




// render
////////////////////////////////////////////////////////////////////////////////
void Grid_layout_simple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    for (auto i=0ull; i < m_drawables.size(); ++i) {
        target.draw(*m_drawables[i], states.transform * m_transforms[i]);
    }
}




// modify
////////////////////////////////////////////////////////////////////////////////
void Grid_layout_simple::add(sf::Drawable const& drawable)
{
    auto const cell_n = m_drawables.size();
    unsigned const row = std::truncf(cell_n/m_column_count);
    unsigned const col = std::truncf(cell_n - row*m_column_count);

    sf::Transform transform;
    transform.translate(col * m_cell_size.x, row * m_cell_size.y);

    m_transforms.push_back(std::move(transform));
    m_drawables.push_back(&drawable);
}




// access
////////////////////////////////////////////////////////////////////////////////
sf::Drawable const& Grid_layout_simple::operator[](std::size_t index) const
{
    return *m_drawables[index];
}




auto Grid_layout_simple::operator[](std::size_t index) -> assignable_cell_t
{
    return assignable_cell_t{this, index};
}




sf::Transform const& Grid_layout_simple::transform_at(std::size_t index) const
{
    return m_transforms[index];
}




std::size_t Grid_layout_simple::size() const
{
    return m_drawables.size();
}
