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
#include <functional>
#include <iostream>
#include <vector>

// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/easing.hpp"
#include "easing/Interpolation.hpp"
#include "sample/Colored_square.hpp"
#include "sample/Slider.hpp"




// Sample configuration
////////////////////////////////////////////////////////////////////////////////
auto const              pause_at_end        = sf::milliseconds(500);
auto constexpr          slider_length       = 640;
static sf::Color const  start_color         = sf::Color::White;
static sf::Color const  end_color           = sf::Color::Black;
struct Keys {
    Keyb                pause_anim          = Keyb::Space;
    Keyb                reset_anim          = Keyb::R;
    Keyb                next_frame          = Keyb::N;
    Keyb                shorten_anim        = Keyb::Subtract;
    Keyb                lengthen_anim       = Keyb::Add;
} constexpr keys;




// Map easing functions to their name string
////////////////////////////////////////////////////////////////////////////////
constexpr std::array<easing_fn_t, 4> easing_functions
{
    easing_linear,
    easing_linear_square,
    easing_linear_cube,
    easing_easy
};


std::array<sf::String, 4> const easing_names
{
    "linear",
    "square",
    "cube",
    "easy"
};



// Control animation state
////////////////////////////////////////////////////////////////////////////////
struct Animation
{
    sf::Clock             clock;
    sf::Time              paused_at;
    float                 duration    = 1.f;
    bool                  reached_end = false;

private:
    std::function<void()> m_reset_function;
    sf::Clock             m_pause_clock;
    bool                  m_paused          = false;

public:
    void reset()        { std::invoke(m_reset_function); clock.restart(); }
    void toggle_pause() { m_paused = !m_paused; m_pause_clock.restart(); }

    auto is_paused() const  { return m_paused; }
    auto pause_time() const { return m_pause_clock.getElapsedTime(); }

    template<typename Fn>
    void set_reset_function(Fn fn) { m_reset_function = fn; }

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




// Sliders sample
////////////////////////////////////////////////////////////////////////////////
    std::vector<Slider> sliders(easing_functions.size(), Slider{slider_length});

    std::vector<Interpolation> sliders_interpolations;

    auto const reset_sliders
        = [&sliders, &sliders_interpolations](float anim_duration)
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

    auto const reset_squares
        = [&squares, &squares_interpolations](float anim_duration)
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
    Animation animation;
    animation.set_reset_function([&animation, &reset_squares, &reset_sliders] {
        reset_squares(animation.duration);
        reset_sliders(animation.duration);
    });




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
        duration_value_text.setString(
            std::to_string(val).substr(0, 4) + " seconds");
    };

    update_duration_text(animation.duration);


    Grid_layout_simple duration_texts_layout {2, {360.f, 0.f}};
    duration_texts_layout.add(duration_text);
    duration_texts_layout.add(duration_value_text);


    std::vector<sf::Text> easing_names_texts (easing_names.size());
    std::transform(
        easing_names.cbegin(), easing_names.cend(),
        easing_names_texts.begin(),
        [&](auto const& easing_name) { return make_text(easing_name); }
    );


    Grid_layout_simple samples_layout {3, {148.f, 128.f}};
    for (size_t i=0; i < sliders.size(); ++i)
    {
        samples_layout.add(easing_names_texts[i]);
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
                    animation.toggle_pause();
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
        if (animation.reached_end && animation.pause_time() > pause_at_end)
        {
            animation.reached_end = false;
            animation.toggle_pause();
            animation.reset();
        }

        else if (!animation.reached_end && !animation.is_paused())
        {
            for (size_t i=0; i < sliders.size(); ++i)
            {
                auto& slider = sliders[i];

                auto& interpolate = sliders_interpolations[i];
                slider.set_circle_position(interpolate(frame_seconds));

                if (slider.ratio() >= 1.f) {
                    animation.reached_end = true;
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
                animation.toggle_pause();
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
