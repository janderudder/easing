// Utilities and libraries
////////////////////////////////////////////////////////////////////////////////
#include "sfext/Grid_layout_simple.hpp"
#include "sfext/Mouse_controller.hpp"
#include "sfext/sf.keyboard.ext.hpp"
#include "sfext/Window.hpp"
#include "util/echo.hpp"
#include "util/filesystem.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>



// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/easing.hpp"
#include "easing/Interpolation.hpp"
#include "sample/Colored_square.hpp"
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
    auto const make_name_text = [&font](sf::String const& str)
    {
        sf::Text t{str, font};
        t.setFillColor(sf::Color::White);
        t.setOutlineColor(sf::Color::Black);
        t.setOutlineThickness(2.f);
        return t;
    };



////////////////////////////////////////////////////////////////////////////////



    // Config sample
    auto constexpr  slider_length       = 768u;
    auto const      pause_at_end_time   = sf::milliseconds(500);
    float constexpr animation_duration  = 1.5f;
    static sf::Color const start_color  = sf::Color::Black;
    static sf::Color const end_color    = sf::Color::White;


    std::vector<easing_fn_t> easing_functions
    {
        easing_linear,
        easing_linear_square,
        easing_linear_cube,
        easing_easy
    };


    std::vector<sf::Text> easing_name_texts
    {
        make_name_text("linear"),
        make_name_text("square"),
        make_name_text("cube"),
        make_name_text("easy")
    };



    std::vector<Slider> sliders(easing_functions.size(), Slider{slider_length});

    std::vector<Colored_square> squares (easing_functions.size(), start_color);


    std::vector<Interpolation> active_interpolations;


    auto const reset_sample_animation
        = [&active_interpolations, &sliders, &squares, &easing_functions]
    {
        active_interpolations.clear();

        for (size_t i=0; i < sliders.size(); ++i)
        {
            sliders[i].set_ratio(0.f);
            active_interpolations.emplace_back(
                easing_functions[i], 0.f, slider_length, animation_duration);
        }

        for (size_t i=0; i < squares.size(); ++i)
        {
            squares[i].set_color(start_color);

            active_interpolations.emplace_back(
                easing_functions[i], start_color.r, end_color.r, animation_duration);

            active_interpolations.emplace_back(
                easing_functions[i], start_color.g, end_color.g, animation_duration);

            active_interpolations.emplace_back(
                easing_functions[i], start_color.b, end_color.b, animation_duration);
        }
    };

    reset_sample_animation();


    // global-animation state
    bool anim_reached_end = false;
    sf::Clock anim_clock;



////////////////////////////////////////////////////////////////////////////////
    Grid_layout_simple layout {3, {/* float(slider_length) + 64.f */128.f, 128.f}};

    for (size_t i=0; i < sliders.size(); ++i) {
        layout.add(easing_name_texts[i]);
        layout.add(squares[i]);
        layout.add(sliders[i]);
    }

    layout.move(88, 128);




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
                case Keyb::Enter:
                case Keyb::Escape:
                    window.close();
                break;

                case Keyb::R:
                    reset_sample_animation();
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


        // --> moving sliders <--
        if (anim_reached_end && anim_clock.getElapsedTime() > pause_at_end_time)
        {
            anim_reached_end = false;
            reset_sample_animation();
        }

        else if (!anim_reached_end)
        {
            anim_reached_end = true; // maybe

            for (size_t i=0; i < sliders.size(); ++i)
            {
                auto& slider = sliders[i];

                if (slider.ratio() < 1.f)   // this accounts for sliders finishing at different times (which should not happen)
                {
                    anim_reached_end = false;

                    auto& interpolate = active_interpolations[i];
                    slider.set_circle_position(interpolate(frame_seconds));
                }
            }

            auto const interp_index_offset = sliders.size();
            for (size_t i=0; i < squares.size(); ++i)
            {
                squares[i].set_color({
                    (sf::Uint8)active_interpolations[interp_index_offset+i](frame_seconds),
                    (sf::Uint8)active_interpolations[interp_index_offset+i+1](frame_seconds),
                    (sf::Uint8)active_interpolations[interp_index_offset+i+2](frame_seconds)
                });
            }

            if (anim_reached_end) {
                anim_clock.restart();
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
