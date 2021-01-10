#pragma once
#include <functional>

using EasingFn = std::function<float(float)>;

float interpolate(EasingFn const&, float progress, float start, float finish);
