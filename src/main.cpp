// Utilities and libraries
////////////////////////////////////////////////////////////////////////////////
#include "sfext/Grid_layout_simple.hpp"
#include "sfext/Mouse_controller.hpp"
#include "sfext/sf.keyboard.ext.hpp"
#include "sfext/Window.hpp"
#include "util/echo.hpp"
#include "util/filesystem.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <vector>



// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/easing.hpp"
#include "easing/Interpolation.hpp"
#include "sample/Colored_square.hpp"
#include "sample/Slider.hpp"



// Configure sample
////////////////////////////////////////////////////////////////////////////////
static float            animation_duration  = 1.1f;
auto const              pause_at_end        = sf::milliseconds(500);

auto constexpr          slider_length       = 640;

static sf::Color const  start_color         = sf::Color::Black;
static sf::Color const  end_color           = sf::Color::White;

auto const              key_reset_anim      = Keyb::R;
auto const              key_pause_anim      = Keyb::Space;



// Set easing functions
////////////////////////////////////////////////////////////////////////////////
std::vector<std::tuple<easing_fn_t, sf::String>> const named_easing_functions
{
    {easing_linear,         "linear"},
    {easing_linear_square,  "square"},
    {easing_linear_cube,    "cube"},
    {easing_easy,           "easy"}
};




int main([[maybe_unused]]const int argc, const char** argv)
////////////////////////////////////////////////////////////////////////////////
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


    std::vector<easing_fn_t> easing_functions (named_easing_functions.size());
    std::transform(
        named_easing_functions.cbegin(),
        named_easing_functions.cend(),
        easing_functions.begin(),
        [](auto const& tuple){ return std::get<easing_fn_t>(tuple); }
    );



// Sliders sample
////////////////////////////////////////////////////////////////////////////////
    std::vector<Slider> sliders(easing_functions.size(), Slider{slider_length});

    std::vector<Interpolation> sliders_interpolations;

    auto const reset_sliders
        = [&sliders, &sliders_interpolations, &easing_functions]
    {
        sliders_interpolations.clear();

        for (size_t i=0; i < sliders.size(); ++i)
        {
            sliders[i].set_ratio(0.f);
            sliders_interpolations.emplace_back(
                easing_functions[i], 0.f, slider_length, animation_duration);
        }
    };

    reset_sliders();



// Squares sample
////////////////////////////////////////////////////////////////////////////////
    std::vector<Colored_square> squares (easing_functions.size(), start_color);

    std::vector<Interpolation> squares_interpolations;

    auto const reset_squares
        = [&squares, &squares_interpolations, &easing_functions]
    {
        squares_interpolations.clear();

        for (size_t i=0; i < squares.size(); ++i)
        {
            squares[i].set_color(start_color);

            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.r),
                float(end_color.r),
                animation_duration
            );
            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.g),
                float(end_color.g),
                animation_duration
            );
            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.b),
                float(end_color.b),
                animation_duration
            );
        }
    };

    reset_squares();



    // global-animation state
    sf::Clock anim_clock;
    bool anim_reached_end = false;
    bool anim_paused      = false;



////////////////////////////////////////////////////////////////////////////////


    auto const make_name_text = [&font](sf::String const& str)
    {
        sf::Text t{str, font};
        t.setFillColor(sf::Color::White);
        t.setOutlineColor(sf::Color::Black);
        t.setOutlineThickness(2.f);
        return t;
    };


    std::vector<sf::Text> easing_name_texts (easing_functions.size());
    std::transform(
        named_easing_functions.cbegin(),
        named_easing_functions.cend(),
        easing_name_texts.begin(),
        [&](auto const& named_fn) {
            return make_name_text(std::get<sf::String>(named_fn));
        }
    );


    Grid_layout_simple layout {3, {148.f, 128.f}};
    for (size_t i=0; i < sliders.size(); ++i)
    {
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

                case key_reset_anim:
                    reset_sliders();
                    reset_squares();
                break;

                case key_pause_anim:
                    anim_paused = !anim_paused;
                break;

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


        // animating samples
        if (anim_reached_end && anim_clock.getElapsedTime() > pause_at_end)
        {
            anim_reached_end = false;
            reset_sliders();
            reset_squares();
        }

        else if (!anim_reached_end && !anim_paused)
        {
            anim_reached_end = true; // maybe

            for (size_t i=0; i < sliders.size(); ++i)
            {
                auto& slider = sliders[i];

                if (slider.ratio() < 1.f) // this...
                // accounts for sliders finishing at different times
                // (which should not happen)
                {
                    anim_reached_end = false;

                    auto& interpolate = sliders_interpolations[i];
                    slider.set_circle_position(interpolate(frame_seconds));
                }
            }


            for (size_t i=0; i < squares.size(); ++i)
            {
                if (squares[i].color() != end_color)
                {
                    squares[i].set_color({
                        sf::Uint8(squares_interpolations[i](frame_seconds)),
                        sf::Uint8(squares_interpolations[i+1](frame_seconds)),
                        sf::Uint8(squares_interpolations[i+2](frame_seconds)),
                        255
                    });
                }
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
