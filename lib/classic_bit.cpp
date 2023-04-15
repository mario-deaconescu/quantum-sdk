#include "../include/classic_bit.hpp"

namespace QPP {

    ClassicBit::ClassicBit() {
        state = ZERO;
    }

    ClassicBit::ClassicBit(const State &state_) {
        state = state_;
    }

    ClassicBit::ClassicBit(bool state_) {
        state = state_ ? ONE : ZERO;
    }

    void ClassicBit::setState(const State &state_) {
        state = state_;
    }

    const ClassicBit::State &ClassicBit::getState() const {
        return state;
    }

    std::string ClassicBit::getRepresentation() const {
        return state == ZERO ? "0" : "1";
    }

}