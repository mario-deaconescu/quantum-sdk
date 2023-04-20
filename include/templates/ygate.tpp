template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CYGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "Y", XGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::YGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "Y", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::YGate::YGate(const size_t &qubitIndex): SingleTargetGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::CYGate::CYGate(const size_t &controlQubitIndex, const size_t &qubitIndex): Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex), Circuit<FloatingNumberType>::XGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::YGate::getRepresentation() const {
    return "Y[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::CYGate::getRepresentation() const {
    return "CY[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::XGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::YGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[SingleTargetGate::qubitIndex];
    const auto newAlpha = targetQubit.getState().getBeta();
    const auto newBeta = -targetQubit.getState().getAlpha();
    targetQubit.setState(newAlpha, newBeta);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CYGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::YGate::apply(circuit);
    }
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::YGate::clone() const {
    return std::make_unique<YGate>(*this);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CYGate::clone() const {
    return static_cast<std::unique_ptr<YGate>>(std::make_unique<CYGate>(*this));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::YGate::verify(const Circuit* circuit) const {
    if(SingleTargetGate::qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(SingleTargetGate::qubitIndex);
    }
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CYGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::YGate::verify(circuit);
}