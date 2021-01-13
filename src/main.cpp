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



    sf::RectangleShape player {{64, 64}};
    player.setOutlineColor(sf::Color::Black);
    player.setOutlineThickness(2.f);
    player.setFillColor(sf::Color::White);


    Motion player_motion {.max_speed = 324.f};

    constexpr float acceleration = 300.f;
    constexpr float deceleration = 300.f;



    Graphics_container gfx { player };

    window.proportional_view().setCenter(player.getPosition());




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

                else if (is_one_of_control_keys(code)
                    && !is_actively_moving(player_motion))
                {
                    player_motion.state = Motion::Accelerating;
                }

                else if (code == Keyb::M)
                {
                    player_motion.state = Motion::Accelerating;
                    player_motion.direction = {1.f, 0.f};
                    std::cout << "Entered \"Accelerating\" state.\n";
                }

                else if (code == Keyb::L)
                {
                    player_motion.state = Motion::Decelerating;
                    std::cout << "Entered \"Decelerating\" state.\n";
                }

                else if (code == Keyb::C)
                {
                    window.proportional_view().setCenter(player.getPosition());
                }
            }

            else if (event.type == sf::Event::KeyReleased)
            {
                auto const& code = event.key.code;

                if (is_one_of_control_keys(code)
                    && is_actively_moving(player_motion))
                {
                    player_motion.state = Motion::Decelerating;
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


        // player motion
        if (player_motion.state == Motion::Accelerating)
        {
            if (player_motion.speed < player_motion.max_speed)
            {
                player_motion.speed += frame_seconds * acceleration;
            }

            else
            {
                player_motion.state = Motion::Cruising;
                player_motion.speed = player_motion.max_speed;
                std::cout << "Entered \"Cruising\" state.\n";
            }
        }

        else if (player_motion.state == Motion::Decelerating)
        {
            if (player_motion.speed > 0.f)
            {
                player_motion.speed -= frame_seconds * deceleration;
            }

            else
            {
                player_motion.state = Motion::Halted;
                player_motion.speed = 0.f;
                std::cout << "Entered \"Halted\" state.\n";
            }
        }

        if (player_motion.state != Motion::Halted)
        {
            auto const distance = player_motion.speed * frame_seconds;

            player.move(
                player_motion.direction.horizontal * distance,
                player_motion.direction.vertical * distance
            );
        }


        // Rendering
        window.clear({172, 215, 255});
        window.set_proportional_view();

        window.draw(gfx);

        window.display();

    }

    return 0;

}
