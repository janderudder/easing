#include "sfext/Window.hpp"
#include <SFML/Window/Mouse.hpp>


// conversion
////////////////////////////////////////////////////////////////////////////////
sf::RenderWindow const& Window::render_window() const
{
    return static_cast<sf::RenderWindow const&>(*this);
}




sf::RenderWindow & Window::render_window()
{
    return static_cast<sf::RenderWindow&>(*this);
}




// constructors
////////////////////////////////////////////////////////////////////////////////
Window::Window(
      sf::VideoMode                 video_mode
    , sf::String const&             title
    , title_bar_style_t             style
    , sf::ContextSettings const&    context
) noexcept
    : sf::RenderWindow              {video_mode, title, style, context}
{
    this->onCreate();
}




Window::Window(
      sf::WindowHandle              handle
    , sf::ContextSettings const&    context
) noexcept
    : sf::RenderWindow              {handle, context}
{
    this->onCreate();
}




// callbacks
////////////////////////////////////////////////////////////////////////////////
#include <cmath>
void Window::onResize()
{
    sf::RenderWindow::onResize();
    auto const [x, y] = getSize();

    m_fixed_view.setCenter(x/2.f, y/2.f);
    m_fixed_view.setSize(x, y);

    auto const [vcx, vcy] = m_proportional_view.getCenter();
    m_proportional_view.setSize(x, y);
}




void Window::onCreate()
{
    sf::RenderWindow::onCreate();
    m_proportional_view = getDefaultView();
    this->onResize();
}




// window API
////////////////////////////////////////////////////////////////////////////////
void Window::create(
      sf::VideoMode                 video_mode
    , sf::String const&             title
    , title_bar_style_t             style
    , sf::ContextSettings const&    context
) noexcept
{
    sf::RenderWindow::create(video_mode, title, style, context);
    onCreate();
}




void Window::create(
      sf::WindowHandle              handle
    , sf::ContextSettings const&    context
) noexcept
{
    sf::RenderWindow::create(handle, context);
    onCreate();
}




bool Window::is_open()
{
    return isOpen();
}




bool Window::has_focus()
{
    return hasFocus();
}




bool Window::poll_event(sf::Event& event)
{
    return pollEvent(event);
}




void Window::close()
{
    sf::RenderWindow::close();
}




void Window::clear(sf::Color const& color)
{
    sf::RenderWindow::clear(color);
}




void Window::draw(sf::Drawable const& drawable, sf::RenderStates const& states)
{
    sf::RenderWindow::draw(drawable, states);
}




void Window::draw(
      sf::VertexBuffer const&   vertex_buffer
    , sf::RenderStates const&   states
){
    sf::RenderWindow::draw(vertex_buffer, states);
}




void Window::draw(
      sf::VertexBuffer const&   vertex_buffer
    , std::size_t               vertex_start
    , std::size_t               vertex_count
    , sf::RenderStates const&   states
){
    sf::RenderWindow::draw(vertex_buffer, vertex_start, vertex_count, states);
}




void Window::draw(
      sf::Vertex const*         vertices
    , std::size_t               vertex_count
    , sf::PrimitiveType         primitive_type
    , sf::RenderStates const&   states
){
    sf::RenderWindow::draw(vertices, vertex_count, primitive_type, states);
}




void Window::display()
{
    sf::RenderWindow::display();
}




void Window::request_focus()
{
    requestFocus();
}




void Window::set_framerate(unsigned rate)
{
    setFramerateLimit(rate);
}




void Window::set_vsync(bool b)
{
    setVerticalSyncEnabled(b);
}




void Window::set_active(bool b)
{
    setActive(b);
}



////////////////////////////////////////////////////////////////////////////////
void Window::set_fixed_view()
{
    setView(m_fixed_view);
}




void Window::set_proportional_view()
{
    setView(m_proportional_view);
}




void Window::move_proportional_view(sf::Vector2f offset)
{
    m_proportional_view.move(offset);
}




void Window::set_proportional_view_position(sf::Vector2f position)
{
    auto const [wcx, wcy] = getSize() / 2u;
    m_proportional_view.setCenter(position.x+wcx, position.y+wcy);
}




void Window::set_view(sf::View const& view)
{
    setView(view);
}




////////////////////////////////////////////////////////////////////////////////
void Window::mouse_cursor_hide()
{
    set_mouse_cursor_visibility(false);
}




void Window::mouse_cursor_show()
{
    set_mouse_cursor_visibility(true);
}




void Window::mouse_cursor_toggle_visibility()
{
    set_mouse_cursor_visibility(!m_is_mouse_cursor_visible);
}




void Window::set_mouse_cursor_visibility(bool cursor_visibility)
{
    m_is_mouse_cursor_visible = cursor_visibility;
    setMouseCursorVisible(m_is_mouse_cursor_visible);
}




////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Window::map_pixel_to_coords(sf::Vector2i const& point) const
{
    return mapPixelToCoords(point);
}




sf::Vector2f Window::map_pixel_to_coords(
      sf::Vector2i const&   point
    , sf::View const&       view
) const
{
    return mapPixelToCoords(point, view);
}




sf::Vector2i Window::map_coords_to_pixel(sf::Vector2f const& point) const
{
    return mapCoordsToPixel(point);
}




sf::Vector2i Window::map_coords_to_pixel(
      sf::Vector2f const&   point
    , sf::View const&       view
) const
{
    return mapCoordsToPixel(point, view);
}




// getters
////////////////////////////////////////////////////////////////////////////////
sf::Vector2u Window::size() const
{
    return getSize();
}




sf::View const& Window::fixed_view() const
{
    return m_fixed_view;
}




sf::View const& Window::proportional_view() const
{
    return m_proportional_view;
}




sf::View& Window::proportional_view()
{
    return m_proportional_view;
}




sf::View const& Window::view() const
{
    return getView();
}




sf::Vector2i Window::mouse_position() const
{
    return sf::Mouse::getPosition(*this);
}





sf::Vector2f Window::mouse_world_position() const
{
    return mapPixelToCoords(sf::Mouse::getPosition(*this));
}




bool Window::is_mouse_cursor_visible() const
{
    return m_is_mouse_cursor_visible;
}
