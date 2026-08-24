#pragma once

#include <nanogui/common.h>
#include <chrono>
#include <cmath>
#include <limits>
#include <stdexcept>

NAMESPACE_BEGIN(nanogui)

/**
 * \brief Estimates the rate of recurring events, e.g. frames per second
 *
 * Call \ref tick() whenever an event occurs. The estimate is an exponentially
 * decayed event count divided by the exponentially decayed time spanned by
 * those events, with a window of ``tau`` seconds of wall-clock time. It
 * therefore settles within a few ``tau`` regardless of how often events occur.
 * Each event may further report a *busy* time, whose decayed average is
 * available via \ref busy().
 */
template <typename Value> class RateMeter {
public:
    /// Construct with the given window time constant (in seconds)
    RateMeter(Value tau = 0.5) : m_tau(tau) {
        if (!(tau > 0))
            throw std::invalid_argument("RateMeter: tau must be positive");
        reset();
    }

    /// Forget all recorded events
    void reset() {
        m_count = m_time = m_busy = 0;
        m_last = std::numeric_limits<Value>::quiet_NaN();
    }

    /**
     * \brief Record an event at the current time
     *
     * ``busy`` optionally specifies the time (in seconds) actively spent
     * producing the event, e.g. excluding waits. \ref busy() reports its
     * decayed average.
     */
    void tick(Value busy = 0) {
        Value now = std::chrono::duration<Value>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        // False on the first call (m_last is NaN)
        if (now > m_last) {
            Value dt = now - m_last,
                  decay = std::exp(-dt / m_tau);
            m_count = std::fma(decay, m_count, Value(1));
            m_time = std::fma(decay, m_time, dt);
            m_busy = std::fma(decay, m_busy, busy);
        }
        m_last = now;
    }

    /// Events per second, or zero before the second event
    Value rate() const { return m_time > 0 ? m_count / m_time : 0; }

    /// Seconds per event, or zero before the second event
    Value interval() const { return m_count > 0 ? m_time / m_count : 0; }

    /// Busy time per event in seconds, or zero before the second event
    Value busy() const { return m_count > 0 ? m_busy / m_count : 0; }

    /// Get the window time constant
    Value tau() const { return m_tau; }

private:
    Value m_tau;
    Value m_count;
    Value m_time;
    Value m_busy;
    Value m_last;
};

NAMESPACE_END(nanogui)
