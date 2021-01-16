#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <initializer_list>
#include <vector>
namespace sfml { class RenderTarget; }


class Vertex_array
    : public sf::Drawable
    , public sf::Transformable
{
    std::vector<sf::Vertex> m_vertices;
    sf::PrimitiveType       m_primitive_type;

public: // construct
    Vertex_array(
          sf::PrimitiveType primitive_type
        , std::size_t       vertex_count    = 0
    );

    Vertex_array(
          sf::PrimitiveType primitive_type
        , std::initializer_list<sf::Vertex>
    );

public: // container modify API
    void clear();
    template <class...Args> void emplace_back(Args&&...);
    void push_back(sf::Vertex const&);
    void push_back(sf::Vertex&&);
    void reserve(std::size_t);
    void resize(std::size_t);
    void resize(std::size_t, sf::Vertex const&);

public: // graphical object API
    void set_color(sf::Color);
    void set_primitive_type(sf::PrimitiveType);

public: // container access API
    sf::Vertex const& operator[](std::size_t) const;
    sf::Vertex &      operator[](std::size_t);
    sf::Vertex const& back() const;
    sf::Vertex &      back();
    sf::Vertex const& front() const;
    sf::Vertex &      front();

public: // info
    bool              empty() const;
    std::size_t       size() const;
    sf::PrimitiveType primitive_type() const;

public: // geometric info
    sf::FloatRect local_bounds() const;
    sf::FloatRect global_bounds() const;

private:
    virtual void draw(
        sf::RenderTarget&,
        sf::RenderStates    = sf::RenderStates::Default
    ) const override;

public:
    using iterator
            = typename std::vector<sf::Vertex>::iterator;
    using const_iterator
            = typename std::vector<sf::Vertex>::const_iterator;
    using reverse_iterator
            = typename std::vector<sf::Vertex>::reverse_iterator;
    using const_reverse_iterator
            = typename std::vector<sf::Vertex>::const_reverse_iterator;

    auto begin()            -> iterator;
    auto end()              -> iterator;
    auto begin()   const    -> const_iterator;
    auto end()     const    -> const_iterator;
    auto cbegin()  const    -> const_iterator;
    auto cend()    const    -> const_iterator;
    auto rbegin()           -> reverse_iterator;
    auto rend()             -> reverse_iterator;
    auto crbegin() const    -> const_reverse_iterator;
    auto crend()   const    -> const_reverse_iterator;

};




// inline impl.
 template <class... Args>
void Vertex_array::emplace_back(Args&&... args)
{
    m_vertices.emplace_back(std::forward<Args>(args)...);
}
