#pragma once
#include <SFML/Graphics/RenderWindow.hpp>



// The class
////////////////////////////////////////////////////////////////////////////////
class Window
    : private sf::RenderWindow
{
    sf::View            m_fixed_view;
    sf::View            m_proportional_view;
    bool                m_is_mouse_cursor_visible   {true};

public:
    using title_bar_style_t = sf::Uint32;

public:
    // conversion
    sf::RenderWindow const& render_window()  const;
    sf::RenderWindow &      render_window();

    // constructors
    Window() = default;

    Window(sf::VideoMode, sf::String const&, title_bar_style_t
        , sf::ContextSettings const& =sf::ContextSettings{}) noexcept;

    Window(sf::WindowHandle
        , sf::ContextSettings const& =sf::ContextSettings{}) noexcept;

    // callbacks
    virtual void onResize();
    virtual void onCreate();

    // window API
    void create(sf::VideoMode, sf::String const&, title_bar_style_t
        , sf::ContextSettings const& =sf::ContextSettings{}) noexcept;
    void create(sf::WindowHandle
        , sf::ContextSettings const& =sf::ContextSettings{}) noexcept;
    bool is_open();
    bool has_focus();
    bool poll_event(sf::Event&);
    void close();
    void clear(sf::Color const& =sf::Color::Black);
    void draw(
          sf::Drawable const&
        , sf::RenderStates const& =sf::RenderStates::Default);
    void draw(
          sf::VertexBuffer const&
        , sf::RenderStates const& =sf::RenderStates::Default);
    void draw(
          sf::VertexBuffer const&
        , std::size_t
        , std::size_t
        , sf::RenderStates const& =sf::RenderStates::Default);
    void draw(
          sf::Vertex const*
        , std::size_t
        , sf::PrimitiveType
        , sf::RenderStates const& =sf::RenderStates::Default);
    void display();
    void request_focus();
    void set_active(bool);
    void set_framerate(unsigned);
    void set_vsync(bool);

    void set_fixed_view();
    void set_proportional_view();
    void move_proportional_view(sf::Vector2f);
    void set_proportional_view_position(sf::Vector2f);
    void set_view(sf::View const&);

    void mouse_cursor_hide();
    void mouse_cursor_show();
    void mouse_cursor_toggle_visibility();
    void set_mouse_cursor_visibility(bool);

    sf::Vector2f map_pixel_to_coords(sf::Vector2i const&) const;
    sf::Vector2f map_pixel_to_coords(sf::Vector2i const&, sf::View const&) const;
    sf::Vector2i map_coords_to_pixel(sf::Vector2f const&) const;
    sf::Vector2i map_coords_to_pixel(sf::Vector2f const&, sf::View const&) const;

    // getters
    sf::Vector2u            size()                      const;
    sf::View const&         fixed_view()                const;
    sf::View const&         proportional_view()         const;
    sf::View&               proportional_view();
    sf::View const&         view()                      const;
    sf::Vector2i            mouse_position()            const;
    sf::Vector2f            mouse_world_position()      const;
    bool                    is_mouse_cursor_visible()   const;

};
