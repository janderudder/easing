#include "sfext/Vertex_array.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>



Vertex_array::Vertex_array(
      sf::PrimitiveType primitive_type
    , std::size_t       vertex_count
)
    : m_vertices        (vertex_count)
    , m_primitive_type  {primitive_type}
{
}




Vertex_array::Vertex_array(
      sf::PrimitiveType                 primitive_type
    , std::initializer_list<sf::Vertex> vertices
)
    : m_vertices        {vertices}
    , m_primitive_type  {primitive_type}
{
}




// container API
////////////////////////////////////////////////////////////////////////////////
void Vertex_array::clear()
{
    m_vertices.clear();
}




void Vertex_array::push_back(sf::Vertex const& vertex)
{
    m_vertices.push_back(vertex);
}




void Vertex_array::push_back(sf::Vertex&& vertex)
{
    m_vertices.push_back(std::move(vertex));
}




void Vertex_array::reserve(std::size_t size)
{
    m_vertices.reserve(size);
}




void Vertex_array::resize(std::size_t size)
{
    m_vertices.resize(size);
}




void Vertex_array::resize(std::size_t size, sf::Vertex const& vertex_value)
{
    m_vertices.resize(size, vertex_value);
}



// graphical object API
////////////////////////////////////////////////////////////////////////////////
void Vertex_array::set_color(sf::Color color)
{
    for (auto& vertex : m_vertices) {
        vertex.color = color;
    }
}




void Vertex_array::set_primitive_type(sf::PrimitiveType prim_type)
{
    m_primitive_type = prim_type;
}




// access contained elements
////////////////////////////////////////////////////////////////////////////////
sf::Vertex const& Vertex_array::operator[](std::size_t index) const
{
    return m_vertices[index];
}




sf::Vertex & Vertex_array::operator[](std::size_t index)
{
    return m_vertices[index];
}




sf::Vertex const& Vertex_array::back() const
{
    return m_vertices.back();
}




sf::Vertex & Vertex_array::back()
{
    return m_vertices.back();
}




sf::Vertex const& Vertex_array::front() const
{
    return m_vertices.front();
}




sf::Vertex & Vertex_array::front()
{
    return m_vertices.front();
}




// info
////////////////////////////////////////////////////////////////////////////////
std::size_t Vertex_array::size() const
{
    return m_vertices.size();
}




sf::PrimitiveType Vertex_array::primitive_type() const
{
    return m_primitive_type;
}



// bounds
////////////////////////////////////////////////////////////////////////////////
sf::FloatRect Vertex_array::local_bounds() const
{
    sf::FloatRect bounds {0, 0, 0, 0};

    for (auto const& vertex : std::as_const(m_vertices))
    {
        auto const& [x, y] = vertex.position;
        bounds.left   = std::min(bounds.left, x);
        bounds.top    = std::min(bounds.top, y);
        bounds.width  = std::max(bounds.width, x);
        bounds.height = std::max(bounds.height, y);
    }

    return bounds;
}




sf::FloatRect Vertex_array::global_bounds() const
{
    return this->getTransform().transformRect(this->local_bounds());
}




// rendering
////////////////////////////////////////////////////////////////////////////////
void Vertex_array::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    target.draw(&m_vertices[0], m_vertices.size(), m_primitive_type, states);
}




// range-for support
////////////////////////////////////////////////////////////////////////////////
auto Vertex_array::begin() -> iterator
{
    return std::begin(m_vertices);
}




auto Vertex_array::end() -> iterator
{
    return std::end(m_vertices);
}




auto Vertex_array::begin() const -> const_iterator
{
    return std::begin(m_vertices);
}




auto Vertex_array::end() const -> const_iterator
{
    return std::end(m_vertices);
}




auto Vertex_array::cbegin() const -> const_iterator
{
    return std::cbegin(m_vertices);
}




auto Vertex_array::cend() const -> const_iterator
{
    return std::cend(m_vertices);
}




auto Vertex_array::rbegin() -> reverse_iterator
{
    return std::rbegin(m_vertices);
}




auto Vertex_array::rend() -> reverse_iterator
{
    return std::rend(m_vertices);
}




auto Vertex_array::crbegin() const -> const_reverse_iterator
{
    return std::crbegin(m_vertices);
}




auto Vertex_array::crend() const -> const_reverse_iterator
{
    return std::crend(m_vertices);
}
