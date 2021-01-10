#pragma once


struct Motion
{
    float max_speed;
    float actual_speed = 0.f;
    enum State {Halt, Accelerating, Cruise, Decelerating} state = Halt;
};
