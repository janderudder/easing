#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Grid_layout_simple
    : public sf::Drawable
    , public sf::Transformable
{
    std::vector<sf::Drawable const*> m_drawables;
    std::vector<sf::Transform>       m_transforms;
    unsigned                         m_column_count;
    sf::Vector2f                     m_cell_size;

    class assignable_cell_t
    {   // Models a grid's cell for direct assignment of reference.
        // Returned by Grid_layout_simple::operator[]().
        Grid_layout_simple* parent;
        std::size_t index;

        friend class Grid_layout_simple;
        assignable_cell_t(Grid_layout_simple*, std::size_t) noexcept;

    public:
        Grid_layout_simple& operator=(sf::Drawable const&) noexcept;
        operator sf::Drawable const&() const;
    };

public:
    Grid_layout_simple(
        unsigned     column_count,
        sf::Vector2f cell_size
    ) noexcept;

    void add(sf::Drawable const&);

    sf::Drawable const& operator[](std::size_t) const;
    assignable_cell_t   operator[](std::size_t);

    sf::Transform const& transform_at(std::size_t) const;
    std::size_t size() const;   // cell count

private:
    virtual void draw(
        sf::RenderTarget&,
        sf::RenderStates    = sf::RenderStates::Default
    ) const override;

};
