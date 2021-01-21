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
auto const              pause_at_end        = sf::milliseconds(500);

auto constexpr          slider_length       = 640;

static sf::Color const  start_color         = sf::Color::Black;
static sf::Color const  end_color           = sf::Color::White;


struct Keys
{
    static Keyb const   reset_anim          = Keyb::R;
    static Keyb const   pause_anim          = Keyb::Space;
    static Keyb const   shorten_anim        = Keyb::Subtract;
    static Keyb const   lengthen_anim       = Keyb::Add;
} keys;



// Set easing functions
////////////////////////////////////////////////////////////////////////////////
std::vector<std::tuple<easing_fn_t, sf::String>> const named_easing_functions
{
    {easing_linear,         "linear"},
    {easing_linear_square,  "square"},
    {easing_linear_cube,    "cube"},
    {easing_easy,           "easy"}
};




////////////////////////////////////////////////////////////////////////////////
int main([[maybe_unused]]const int argc, const char** argv)
{
    // Window
    Window window {
        sf::VideoMode{1016, 680},
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

    static auto const reset_sliders
        = [&sliders, &sliders_interpolations, &easing_functions](float anim_duration)
    {
        sliders_interpolations.clear();

        for (size_t i=0; i < sliders.size(); ++i)
        {
            sliders[i].set_ratio(0.f);
            sliders_interpolations.emplace_back(
                easing_functions[i], 0.f, slider_length, anim_duration);
        }
    };



// Squares sample
////////////////////////////////////////////////////////////////////////////////
    std::vector<Colored_square> squares (easing_functions.size(), start_color);

    std::vector<Interpolation> squares_interpolations;

    static auto const reset_squares
        = [&squares, &squares_interpolations, &easing_functions](float anim_duration)
    {
        squares_interpolations.clear();

        for (size_t i=0; i < squares.size(); ++i)
        {
            squares[i].set_color(start_color);

            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.r),
                float(end_color.r),
                anim_duration
            );
            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.g),
                float(end_color.g),
                anim_duration
            );
            squares_interpolations.emplace_back(
                easing_functions[i],
                float(start_color.b),
                float(end_color.b),
                anim_duration
            );
        }
    };



// global-animation state
////////////////////////////////////////////////////////////////////////////////
    struct
    {
        sf::Clock clock;
        bool      reached_end;
        bool      paused;
        float     duration = 1.f;

        void reset() { reset_sliders(duration); reset_squares(duration); }

    } animation;



// Texts and layouts
////////////////////////////////////////////////////////////////////////////////
    auto const make_text = [&font](sf::String const& str)
    {
        sf::Text t{str, font};
        t.setFillColor(sf::Color::White);
        t.setOutlineColor(sf::Color::Black);
        t.setOutlineThickness(2.f);
        return t;
    };


    auto const duration_text = make_text("animation duration: ");
    auto duration_value_text = make_text("");

    auto const update_duration_text = [&duration_value_text](float val)
    {
        duration_value_text.setString(std::to_string(val).substr(0, 4) + " seconds");
    };
    update_duration_text(animation.duration);


    Grid_layout_simple duration_texts_layout {2, {360.f, 0.f}};
        duration_texts_layout.add(duration_text);
        duration_texts_layout.add(duration_value_text);


    std::vector<sf::Text> easing_name_texts (easing_functions.size());
    std::transform(
        named_easing_functions.cbegin(),
        named_easing_functions.cend(),
        easing_name_texts.begin(),
        [&](auto const& named_fn) {
            return make_text(std::get<sf::String>(named_fn));
        }
    );


    Grid_layout_simple samples_layout {3, {148.f, 128.f}};
        for (size_t i=0; i < sliders.size(); ++i)
        {
            samples_layout.add(easing_name_texts[i]);
            samples_layout.add(squares[i]);
            samples_layout.add(sliders[i]);
        }



    Grid_layout_simple layout {1, {0.f, 128.f}};
        layout.add(duration_texts_layout);
        layout.add(samples_layout);

    layout.move(32, 32);



// App loop
////////////////////////////////////////////////////////////////////////////////
    animation.reset();

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

                case keys.reset_anim:
                    animation.reset();
                break;

                case keys.pause_anim:
                    animation.paused = !animation.paused;
                break;

                case keys.shorten_anim:
                    animation.duration = std::max(0.f, animation.duration-0.1f);
                    update_duration_text(animation.duration);
                    animation.reset();
                break;

                case keys.lengthen_anim:
                    animation.duration += .1f;
                    update_duration_text(animation.duration);
                    animation.reset();
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
        if (animation.reached_end && animation.clock.getElapsedTime() > pause_at_end)
        {
            animation.reached_end = false;
            animation.reset();
        }

        else if (!animation.reached_end && !animation.paused)
        {
            animation.reached_end = true; // maybe

            for (size_t i=0; i < sliders.size(); ++i)
            {
                auto& slider = sliders[i];

                if (slider.ratio() < 1.f) // this...
                // accounts for sliders finishing at different times
                // (which should not happen)
                {
                    animation.reached_end = false;

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


            if (animation.reached_end) {
                animation.clock.restart();
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
