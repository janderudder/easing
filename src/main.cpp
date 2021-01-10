// Utilities and libraries
////////////////////////////////////////////////////////////////////////////////
#include "sfext/Graphics_container.hpp"
#include "sfext/Mouse_controller.hpp"
#include "sfext/sf.keyboard.ext.hpp"
#include "sfext/sf.transform.ext.hpp"
#include "sfext/Window.hpp"
#include "util/filesystem.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>



// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/easing.hpp"
#include "easing/easing_functions.hpp"
#include "motion/Motion.hpp"



// Program start
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


    // keyboard utils
    auto const is_one_of_quit_keys = [](sf::Keyboard::Key const& k)
    {
        return k == Keyb::Enter || k == Keyb::Escape || k == Keyb::Space;
    };


    auto const is_one_of_control_keys = [](sf::Keyboard::Key const& k)
    {
        return k == Keyb::Z || k == Keyb::Q || k == Keyb::S || k == Keyb::D;
    };




////////////////////////////////////////////////////////////////////////////////



    sf::RectangleShape e {{64, 64}};
    e.setOutlineColor(sf::Color::Black);
    e.setOutlineThickness(2.f);
    e.setFillColor(sf::Color::White);


    Motion player_motion {324.f};



    constexpr float e_speed = 256.f;



    Graphics_container gfx { e };



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
            {
                auto const& code = event.key.code;

                if (is_one_of_quit_keys(code))
                {
                    window.close();
                }

                else if (is_one_of_control_keys(code))
                {

                }
            }

            else if (event.type == sf::Event::KeyReleased)
            {
                auto const& code = event.key.code;

                if (is_one_of_control_keys(code))
                {

                }
            }

            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }


        // Frame time
        const auto frame_time = clock.restart();
        const auto frame_seconds = frame_time.asSeconds();


        // real-time mouse
        mouse.update(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            window.move_proportional_view(mouse.delta<float>());
        }


        // real-time keyboard
        if (is_pressed(Keyb::Q)) {
            e.move(-e_speed*frame_seconds, 0);
        }
        else if (is_pressed(Keyb::D)) {
            e.move(e_speed*frame_seconds, 0);
        }
        if (is_pressed(Keyb::Z)) {
            e.move(0, -e_speed*frame_seconds);
        }
        else if (is_pressed(Keyb::S)) {
            e.move(0, e_speed*frame_seconds);
        }


        // Rendering
        window.clear({172, 215, 255});
        window.set_proportional_view();

        window.draw(gfx);

        window.display();

    }

    return 0;

}
