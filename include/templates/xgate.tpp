template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CXGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "X", XGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::XGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "X", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::XGate::XGate(const size_t &qubitIndex): SingleTargetGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::CXGate::CXGate(const size_t &controlQubitIndex, const size_t &qubitIndex):
Circuit<FloatingNumberType>::SingleTargetGate(qubitIndex),
Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex),
Circuit<FloatingNumberType>::XGate(qubitIndex) {}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::XGate::getRepresentation() const {
    return "X[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::CXGate::getRepresentation() const {
    return "CX[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::XGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::XGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[SingleTargetGate::qubitIndex];
    const auto newAlpha = targetQubit.getState().getBeta();
    const auto newBeta = targetQubit.getState().getAlpha();
    targetQubit.setState(newAlpha, newBeta);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CXGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::XGate::apply(circuit);
    }
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::XGate::clone() const {
    return std::make_unique<XGate>(*this);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CXGate::clone() const {
    return static_cast<std::unique_ptr<XGate>>(std::make_unique<CXGate>(*this));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::XGate::verify(const Circuit* circuit) const {
    if(SingleTargetGate::qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(SingleTargetGate::qubitIndex);
    }
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CXGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::XGate::verify(circuit);
}