/**
 * @file classic_bit.h
 * @brief This file contains the ClassicBit class.
 * @author Mario Deaconescu
 */

#pragma once

#include <ostream>
#include "representable.hpp"

namespace QPP {

/**
 * @class ClassicBit
 * @brief A class that represents a classic bit.
 */
    class ClassicBit : public Representable {
    public:
        enum State {
            ZERO = 0,
            ONE = 1
        };

        /// \brief Creates a ClassicBit with state 0
        ClassicBit();

        /// \brief Creates a ClassicBit from a State
        /// \param state_ The state of the bit
        explicit ClassicBit(const State &state_);

        /// \brief Creates a ClassicBit from a boolean
        /// \param state_ The state of the bit (true = 1, false = 0)
        explicit ClassicBit(bool state_);

        /// \brief Sets the state of the bit
        /// \param state_ The state to set the bit to
        void setState(const State &state_);

        /// \brief Gets the state of the bit
        /// \return The state of the bit
        [[nodiscard]] const State &getState() const;

        /// \brief Gets the representation of the bit ("0" or "1")
        [[nodiscard]] std::string getRepresentation() const override;

    private:
        /// \brief The state of the bit
        State state; /**< The state of the bit. */
    };

}