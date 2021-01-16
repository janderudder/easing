#pragma once


struct Motion
{
    enum State {
        Halted          = 0b00,
        Accelerating    = 0b01,
        Cruising        = 0b11,
        Decelerating    = 0b10
    };

    float       max_speed;
    float       speed        = 0.f;
    State       state        = Halted;
};



inline bool is_actively_moving(Motion const& m)
{
    return int(m.state) & 0b01;
}
