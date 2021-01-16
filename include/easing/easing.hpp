#pragma once
#include <functional>


using easing_fn_t = float(*)(float);



 template <typename Fn = easing_fn_t>
float interpolate(Fn&&, float progress, float start, float finish);



// Predefined easing functions
float easing_linear(float n);
float easing_linear_square(float n);
float easing_linear_cube(float n);
float easing_easy(float n);




// impl.
////////////////////////////////////////////////////////////////////////////////
 template <typename Fn>
float interpolate(
    Fn&&  easing_fn,
    float const progress,
    float const start,
    float const finish
){
    if (progress <= 0.f) {
        return start;
    }
    else if (progress >= 1.f) {
        return finish;
    }
    else {
        auto const ep = std::invoke(easing_fn, progress);
        // => start * (1 - ep) + finish * ep
        return start + ep * (finish - start);
    }
}



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
