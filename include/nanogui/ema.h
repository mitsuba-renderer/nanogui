#pragma once

#include <nanogui/common.h>
#include <cmath>

NAMESPACE_BEGIN(nanogui)

/// Exponentially weighted moving average accumulator with bias correction
template <typename Value> class EMA {
public:
    /// Construct with given weight for old samples
    /// The default weight (0.983) results in a ~1 second time constant at 60 FPS
    EMA(Value weight = 0.983f)
        : m_weights{weight, 1.f-weight}, m_value(0.f),
          m_sample_count(0) {
        if (weight < 0.f || weight >= 1.f)
            throw std::invalid_argument("Weight must be in range [0, 1)");
    }

    /// Reset the accumulator to initial state
    void reset() { m_value = 0.f; m_sample_count = 0; }

    /// Add a new sample to the accumulator
    void put(Value sample) {
        m_value = std::fma(m_weights[0], m_value, sample * m_weights[1]);
        m_sample_count++;
    }

    /// Get the bias-corrected accumulated value
    Value value() const {
        if (m_sample_count == 0)
            return 0.f;
        return (Value) (m_value / (1.f - std::pow(m_weights[0], m_sample_count)));
    }

    /// Get the current weight
    Value weight() const { return m_weights[0]; }

    /// Get the number of samples accumulated
    size_t sample_count() const { return m_sample_count; }

private:
    Value m_weights[2];
    Value m_value;
    size_t m_sample_count;
};

NAMESPACE_END(nanogui)
