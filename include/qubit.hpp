/**

@file qubit.hpp
@brief Definition of Qubit class template and its nested State class.
@author Mario Deaconescu
*/


#pragma once

#include <complex>
#include <memory>
#include "classic_bit.hpp"
#include "probability.hpp"
#include "representable.hpp"

///@class Qubit
///
///@brief A class template representing a single qubit.
///
///This class template represents a single qubit with a quantum state that can be manipulated and measured.
///
///It contains a nested State class representing the quantum state of the qubit.
///
///@tparam FloatingNumberType The type of the floating-point number used to represent the amplitudes of the qubit state.
template <typename FloatingNumberType>
class Qubit : public Representable{
public:
    ///@class State
    ///
    ///@brief A class representing the quantum state of a qubit.
    ///
    ///This class represents the quantum state of a qubit.
    ///
    ///It contains a nested InvalidStateException class representing an exception thrown when the state is invalid.
    ///
    class State : public Representable{
    private:
        class InvalidStateException : public std::exception {
        public:
            explicit InvalidStateException(const typename Qubit<FloatingNumberType>::State &state);
            const char* what() const noexcept override;
        private:
            typename Qubit<FloatingNumberType>::State state;
            mutable std::string message;
        };
        std::complex<FloatingNumberType> alpha;
        std::complex<FloatingNumberType> beta;
        const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine;
        void assertValid() const;
    public:
        ///@brief Creates a State with the given probability engine.
        ///@param probabilityEngine The probability engine to use.
        explicit State(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine);
        ///@brief Creates a State with the given probability engine and the given state to copy.
        ///@param other The state to copy.
        State(const State& other);
        ///@brief Creates a State with the given probability engine and the given amplitudes.
        ///@param probabilityEngine The probability engine to use.
        ///@param alpha The alpha amplitude.
        ///@param beta The beta amplitude.
        State(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine, const std::complex<FloatingNumberType>& alpha, const std::complex<FloatingNumberType>& beta);

        ///@brief Sets the state to the given state.
        ///@param alpha_ The alpha amplitude.
        ///@param beta_ The beta amplitude.
        void set(const std::complex<FloatingNumberType>& alpha_, const std::complex<FloatingNumberType>& beta_);

        ///@brief Gets the alpha amplitude.
        ///@return The alpha amplitude.
        [[nodiscard]] const std::complex<FloatingNumberType>& getAlpha() const;
        ///@brief Gets the beta amplitude.
        ///@return The beta amplitude.
        [[nodiscard]] const std::complex<FloatingNumberType>& getBeta() const;


        State& operator=(const State& other);

        ///@brief Gets the representation of the state.
        ///@return The representation of the state as the sum of the alpha and beta amplitudes multiplied with the respective kets.
        [[nodiscard]] std::string getRepresentation() const override;
    };

    ///@brief Creates a Qubit with the given probability engine.
    ///@param probabilityEngine The probability engine to use.
    explicit Qubit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine);
    ///@brief Creates a Qubit with the given probability engine and the given state to copy.
    ///@param qubit The qubit to copy.
    Qubit(const Qubit& qubit);

    Qubit& operator=(const Qubit& qubit);

    ///@brief Gets the representation of the qubit.
    ///@return The representation of the qubit as the sum of the alpha and beta amplitudes multiplied with the respective kets.
    [[nodiscard]] std::string getRepresentation() const override;

    ///@brief Applies the given state to the qubit.
    ///@param state_ The state to set the qubit to.
    void setState(const State& state_);
    ///@brief Applies the given state to the qubit.
    ///@param alpha The alpha amplitude.
    ///@param beta The beta amplitude.
    void setState(const std::complex<FloatingNumberType>& alpha, const std::complex<FloatingNumberType>& beta);

    ///@brief Gets the state of the qubit.
    ///@return The state of the qubit.
    [[nodiscard]] const State& getState() const;

    ///@brief Measures the qubit.
    ///
    ///WARNING: This function changes the state of the qubit. (The qubit is collapsed to a single state.)
    ///
    ///@return The measured state of the qubit.
    ClassicBit measure();

private:
    State state;
    const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine;
};

#include "templates/qubit.tpp"