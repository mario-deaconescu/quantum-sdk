template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CZGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "Z", ZGate::qubitIndex);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ZGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "Z", qubitIndex);
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::ZGate::ZGate(const size_t &qubitIndex): qubitIndex(qubitIndex) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CZGate::CZGate(const size_t &controlQubitIndex, const size_t &qubitIndex): Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex), Circuit<FloatingNumberType>::ZGate(qubitIndex) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::ZGate::getRepresentation() const {
    return "Z[Q#" + std::to_string(qubitIndex) + "]";
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::CZGate::getRepresentation() const {
    return "CZ[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::ZGate::qubitIndex) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::ZGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[qubitIndex];
    const auto newBeta = -targetQubit.getState().getBeta();
    targetQubit.setState(targetQubit.getState().getAlpha(), newBeta);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CZGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::ZGate::apply(circuit);
    }
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::ZGate::clone() const {
    return std::make_unique<ZGate>(*this);
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CZGate::clone() const {
    return static_cast<std::unique_ptr<ZGate>>(std::make_unique<CZGate>(*this));
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::ZGate::verify(const Circuit* circuit) const {
    if(qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex);
    }
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CZGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::ZGate::verify(circuit);
}