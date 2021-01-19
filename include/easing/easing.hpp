#pragma once
#include <functional>


////////////////////////////////////////////////////////////////////////////////
using easing_fn_t = float(*)(float);


// Predefined easing functions
////////////////////////////////////////////////////////////////////////////////
float easing_linear(float n);
float easing_linear_square(float n);
float easing_linear_cube(float n);
float easing_easy(float n);



// impl.
////////////////////////////////////////////////////////////////////////////////
inline float easing_linear(float n)
{
    return n;
}



inline float easing_linear_square(float n)
{
    return n * n;
}



inline float easing_linear_cube(float n)
{
    return n * n * n;
}



inline float easing_easy(float n)
{
    return n * n * (3 - 2 * n);
}
