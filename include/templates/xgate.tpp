template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CXGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "X", XGate::qubitIndex);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::XGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "X", qubitIndex);
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::XGate::XGate(const size_t &qubitIndex): qubitIndex(qubitIndex) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CXGate::CXGate(const size_t &controlQubitIndex, const size_t &qubitIndex): Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex), Circuit<FloatingNumberType>::XGate(qubitIndex) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::XGate::getRepresentation() const {
    return "X[Q#" + std::to_string(qubitIndex) + "]";
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::CXGate::getRepresentation() const {
    return "CX[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::XGate::qubitIndex) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::XGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[qubitIndex];
    const auto newAlpha = targetQubit.getState().getBeta();
    const auto newBeta = targetQubit.getState().getAlpha();
    targetQubit.setState(newAlpha, newBeta);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CXGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::XGate::apply(circuit);
    }
}