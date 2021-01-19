/* /////////////////////////////////////////////////////////////////////////////

    class Interpolation

    Callable object.
    Should live as long as the interpolation performs.

    Create one per interpolated variable value.
    Call it each frame with frame's duration.
    It returns the next value of the evolution.

///////////////////////////////////////////////////////////////////////////// */
#pragma once
#include <functional>


////////////////////////////////////////////////////////////////////////////////
class Interpolation
{
    using easing_fn_t = std::function<float(float)>;

    easing_fn_t const   m_easing_fn;
    float const         m_begin_value;
    float const         m_end_value;
    float const         m_duration_seconds;
    float               m_elapsed_seconds;

public:
    using easing_fn_signature = float(float);

        template <typename Fn>
    Interpolation(Fn&&, float beg_val, float end_val, float duration) noexcept;

    float operator()(float seconds) noexcept;

};
////////////////////////////////////////////////////////////////////////////////




// impl.
////////////////////////////////////////////////////////////////////////////////
    template <typename Fn>
Interpolation::Interpolation(
    Fn&&  easing_fn,
    float bv,
    float ev,
    float duration
) noexcept
    : m_easing_fn        {std::forward<Fn>(easing_fn)}
    , m_begin_value      {bv}
    , m_end_value        {ev}
    , m_duration_seconds {duration}
    , m_elapsed_seconds  {0.f}
{
}



////////////////////////////////////////////////////////////////////////////////
inline float Interpolation::operator()(float seconds) noexcept
{
    m_elapsed_seconds += seconds;

    auto const progress = m_elapsed_seconds / m_duration_seconds;

    if (progress <= 0.f) {
        return m_begin_value;
    }
    else if (progress >= 1.f) {
        return m_end_value;
    }
    else {
        auto const ep = std::invoke(m_easing_fn, progress);
        // m_begin_value * (1 - ep) + m_end_value * ep
        return m_begin_value + ep * (m_end_value - m_begin_value);
    }
}
