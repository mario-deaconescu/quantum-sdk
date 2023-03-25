/// @file probability.h
/// @brief This file contains the ProbabilityEngine class template.
/// @author Mario Deaconescu

#pragma once

#include <random>
#include <concepts>

/// @class ProbabilityEngine
/// @brief A class template representing a probability engine.
/// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
template<std::floating_point FloatingNumberType>
class ProbabilityEngine {
private:
    std::random_device device;
    std::uniform_real_distribution<FloatingNumberType> distribution;
    const FloatingNumberType errorMargin = 2e-10;
public:
    /// @brief Creates a ProbabilityEngine with the default error margin.
    ProbabilityEngine();
    /// @brief Creates a ProbabilityEngine with the given error margin.
    /// @param errorMargin_ The error margin to use.
    explicit ProbabilityEngine(const FloatingNumberType& errorMargin_);
    /// @brief Gets a random probability.
    /// @return A random probability from the interval [0, 1].
    FloatingNumberType getProbability();

    /// @brief Compares two numbers using an error margin.
    /// @tparam T The type of the numbers.
    /// @param number1 The first number.
    /// @param number2 The second number.
    /// @return True if the numbers are equal, false otherwise.
    template<class T>
    bool compare(const T& number1, const T& number2) const{
        return std::abs(number1 - number2) < errorMargin;
    }
};

#include "templates/probability.tpp"