#include "easing/easing.hpp"


EasingFn easing_linear
    = [](float n) -> float { return n; };

EasingFn easing_linear_square
    = [](float n) -> float { return n * n; };

EasingFn easing_linear_cube
    = [](float n) -> float { return n * n * n; };

EasingFn easing_easy
    = [](float n) -> float { return n * n * (3 - 2 * n); };
