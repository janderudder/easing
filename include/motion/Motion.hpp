#pragma once


struct Motion
{
    enum State {
        Halted          = 0b00,
        Accelerating    = 0b01,
        Cruising        = 0b11,
        Decelerating    = 0b10
    };

    struct Direction {
        float horizontal;
        float vertical  ;
    };

    float       max_speed;
    float       speed        = 0.f;
    State       state        = Halted;
    Direction   direction    = {0.f, 0.f};
};



inline bool is_actively_moving(Motion const& m)
{
    return int(m.state) & 0b01;
}
