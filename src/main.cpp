// Utilities and libraries
////////////////////////////////////////////////////////////////////////////////
#include "sfext/Graphics_container.hpp"
#include "sfext/Mouse_controller.hpp"
#include "sfext/sf.keyboard.ext.hpp"
#include "sfext/sf.transform.ext.hpp"
#include "sfext/Vertex_array.hpp"
#include "sfext/Window.hpp"
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

    // keyboard
    auto const quit_keys = {Keyb::Enter, Keyb::Escape};


////////////////////////////////////////////////////////////////////////////////



    Slider slider1 {768, 18.f};



    Graphics_container gfx { slider1 };
    gfx.move(64, 64);



    std::vector<Slider*> sliders {&slider1};
    Slider* active_slider = nullptr;
    Slider* mouse_active_slider = nullptr;




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

                if (is_one_of(quit_keys, code)) {
                    window.close();
                }

                else if (code == Keyb::Space)
                {
                    if (active_slider) {
                        active_slider = nullptr;
                    }
                    else
                    {
                        active_slider = &slider1;

                        if (active_slider->ratio() == 1.f)
                        {
                            active_slider->set_ratio(0);
                            active_slider = nullptr;
                        }
                    }
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
        if (mouse_active_slider)
        {
            active_slider->set_circle_position(
                active_slider->circle_position() + -mouse.delta<float>().x
            );
        }


        if (active_slider)
        {
            active_slider->set_circle_position(
                active_slider->circle_position() + 300.f * frame_seconds
            );

            if (active_slider->ratio() == 1.f) {
                active_slider = nullptr;
            }
        }


        // Rendering
        window.clear({172, 215, 255});
        window.set_proportional_view();

        window.draw(gfx);

        window.display();

    }

    return 0;

}
