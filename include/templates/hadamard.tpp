template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::HadamardGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "H", qubitIndex);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ControlledHadamardGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "H", HadamardGate::qubitIndex);
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::HadamardGate::HadamardGate(const size_t &qubitIndex): qubitIndex(qubitIndex) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::ControlledHadamardGate::ControlledHadamardGate(const size_t &controlIndex, const size_t &targetIndex): ControlledGate(controlIndex),
                                                                                                                                    HadamardGate(targetIndex) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::HadamardGate::getRepresentation() const {
    return "H[Q#" + std::to_string(qubitIndex) + "]";
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::ControlledHadamardGate::getRepresentation() const {
    return "CH[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ? " + std::to_string(Circuit<FloatingNumberType>::HadamardGate::qubitIndex) +"]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::HadamardGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& qubit = circuit->qubits[qubitIndex];
    const auto alpha = qubit.getState().getAlpha();
    const auto beta = qubit.getState().getBeta();
    const auto newAlpha = (alpha + beta) / (FloatingNumberType)std::sqrt(2);
    const auto newBeta = (alpha - beta) / (FloatingNumberType)std::sqrt(2);
    qubit.setState(newAlpha, newBeta);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledHadamardGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& qubit = circuit->qubits[Circuit<FloatingNumberType>::ControlledGate::controlIndex];
    if(qubit.measure().getState() == ClassicBit::State::ONE){
        HadamardGate::apply(circuit);
    }
}