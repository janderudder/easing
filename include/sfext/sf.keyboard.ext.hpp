#pragma once
#include <SFML/Window.hpp>


// Alias shortcuts
using Keyb = sf::Keyboard::Key;
constexpr auto const is_pressed = &sf::Keyboard::isKeyPressed;


// Manage modifier keys states and their implications
struct Pressed_modifiers
{
    bool ctrl  {false};
    bool l_alt {false};
    bool r_alt {false};
    bool shift {false};

    void update()
    {
        shift = is_pressed(Keyb::LShift);
        l_alt = is_pressed(Keyb::LAlt);
        r_alt = is_pressed(Keyb::RAlt);
        ctrl  = is_pressed(Keyb::LControl);
    }

    bool are_preventing_text_input() const
    {
        return ctrl && !(l_alt || r_alt);
    }
};
