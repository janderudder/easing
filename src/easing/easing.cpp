#include "easing/easing.hpp"


float interpolate(EasingFn const& easing_fn, float const progress, float const start, float const finish)
{
    if (progress <= 0.f)
    {
        return start;
    }
    else if (progress >= 1.f)
    {
        return finish;
    }
    else
    {
        auto const progi = easing_fn(progress);
        return start + progi * (finish - start);     // factorised from `start * (1 - progi) + finish * progi`
    }
}
