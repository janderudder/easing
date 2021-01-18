// Utilities and libraries
////////////////////////////////////////////////////////////////////////////////
#include "sfext/Graphics_container.hpp"
#include "sfext/Grid_layout_simple.hpp"
#include "sfext/Mouse_controller.hpp"
#include "sfext/sf.keyboard.ext.hpp"
#include "sfext/sf.transform.ext.hpp"
#include "sfext/Vertex_array.hpp"
#include "sfext/Window.hpp"
#include "util/echo.hpp"
#include "util/filesystem.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>



// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/easing.hpp"
#include "sample/Slider.hpp"



////////////////////////////////////////////////////////////////////////////////
int main([[maybe_unused]]const int argc, const char** argv)
{
    // Window
    Window window {
        sf::VideoMode{1366, 768},
        "easing-01",
        sf::Style::Default
    };

    // Config
    window.set_vsync(true);

    // Utility
    Mouse_controller mouse;
    Pressed_modifiers pressed_modifiers;
    std::boolalpha(std::cout);
    std::boolalpha(std::cerr);

    // Time
    sf::Clock   clock;

    // Resources
    auto const resource_path =
        filesystem::canonical(argv[0])
            .remove_filename().parent_path() / "resource";

    sf::Font font;
    font.loadFromFile((resource_path/"font/CutiveMono-Regular.ttf").string());



////////////////////////////////////////////////////////////////////////////////


    auto constexpr slider_length = 768u;


    std::vector<Slider> sliders (2, Slider{slider_length});


    Grid_layout_simple layout {1, {float(slider_length), 128.f}};
    layout.move(88, 128);


    for (auto const& slider : std::as_const(sliders)) {
        layout.add(slider);
    }


    auto const sliders_set_ratios = [&sliders](float ratio)
    {
        for (auto& slider : sliders) {
            slider.set_ratio(ratio);
        }
    };


    class
    {
        bool        m_is_active   = false;
        float const m_speed       = 300.f;
     public:

        float speed() const         { return m_speed; }
        bool is_active() const      { return m_is_active; }
        void set_active(bool b)     { m_is_active = b; }

    } animation;



// App loop
////////////////////////////////////////////////////////////////////////////////
    while (window.is_open())
    {
        // state
        pressed_modifiers.update();

        // events
        for (sf::Event event; window.poll_event(event);)
        {
            if (event.type == sf::Event::KeyPressed)
            {   switch (event.key.code)
                {
                case Keyb::Escape:
                    window.close();
                break;

                case Keyb::Enter:
                    sliders_set_ratios(0.f);
                break;

                case Keyb::Space:
                    animation.set_active(!animation.is_active());
                default:
                break;
                }
            }

            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }


        // frame time
        const auto frame_time = clock.restart();
        const auto frame_seconds = frame_time.asSeconds();


        // real-time mouse
        mouse.update(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            window.move_proportional_view(mouse.delta<float>());
        }


        // moving sliders
        if (animation.is_active())
        {
            bool maybe_animation_finished = true;

            auto const distance = animation.speed() * frame_seconds;

            for (auto& slider : sliders)
            {
                if (slider.ratio() < 1.f)
                {
                    maybe_animation_finished = false;
                    slider.set_circle_position(slider.circle_position() + distance);
                }
            }

            if (maybe_animation_finished)
            {
                sliders_set_ratios(0.f);
            }
        }


        // Rendering
        window.clear({172, 215, 255});
        window.set_proportional_view();

        window.draw(layout);

        window.display();

    }

    return 0;

}
