template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CZGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "Z", ZGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ZGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "Z", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::ZGate::ZGate(const size_t &qubitIndex): SingleTargetGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::CZGate::CZGate(const size_t &controlQubitIndex, const size_t &qubitIndex): Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex), Circuit<FloatingNumberType>::ZGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::ZGate::getRepresentation() const {
    return "Z[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::CZGate::getRepresentation() const {
    return "CZ[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::ZGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::ZGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[SingleTargetGate::qubitIndex];
    const auto newBeta = -targetQubit.getState().getBeta();
    targetQubit.setState(targetQubit.getState().getAlpha(), newBeta);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CZGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::ZGate::apply(circuit);
    }
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::ZGate::clone() const {
    return std::make_unique<ZGate>(*this);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CZGate::clone() const {
    return static_cast<std::unique_ptr<ZGate>>(std::make_unique<CZGate>(*this));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::ZGate::verify(const Circuit* circuit) const {
    if(SingleTargetGate::qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(SingleTargetGate::qubitIndex);
    }
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CZGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::ZGate::verify(circuit);
}