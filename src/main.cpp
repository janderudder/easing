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



// Project specific includes
////////////////////////////////////////////////////////////////////////////////
#include "easing/EasingFn.hpp"



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



    Graphics_container gfx {  };




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
                switch (event.key.code)
                {
                case Keyb::Enter:
                case Keyb::Escape:
                case Keyb::Space:
                    window.close();
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

        // Frame time
        const auto frame_time = clock.restart();
        const auto frame_seconds = frame_time.asSeconds();

        // real-time events
        mouse.update(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            window.move_proportional_view(mouse.delta<float>());
        }

        // Rendering
        window.clear({172, 215, 255});
        window.set_proportional_view();

        window.draw(gfx);

        window.display();

    }

    return 0;

}
