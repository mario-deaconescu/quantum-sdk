template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ControlledPhaseGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "P", PhaseGate::qubitIndex);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::PhaseGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "P", qubitIndex);
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::PhaseGate::PhaseGate(const size_t &qubitIndex, const double& angle): qubitIndex(qubitIndex), angle(angle) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::ControlledPhaseGate::ControlledPhaseGate(const size_t &controlQubitIndex, const size_t &qubitIndex, const double& angle): Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex), Circuit<FloatingNumberType>::PhaseGate(qubitIndex, angle) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::PhaseGate::getRepresentation() const {
    return "P[Q#" + std::to_string(qubitIndex) + "]";
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::ControlledPhaseGate::getRepresentation() const {
    return "CP[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::PhaseGate::qubitIndex) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::PhaseGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[qubitIndex];
    const auto newAlpha = targetQubit.getState().getAlpha();
    const auto newBeta = targetQubit.getState().getBeta() * std::exp(std::complex<FloatingNumberType>(0, angle));
    targetQubit.setState(newAlpha, newBeta);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledPhaseGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::PhaseGate::apply(circuit);
    }
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::PhaseGate::clone() const {
    return std::make_unique<PhaseGate>(*this);
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::ControlledPhaseGate::clone() const {
    return static_cast<std::unique_ptr<PhaseGate>>(std::make_unique<ControlledPhaseGate>(*this));
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::PhaseGate::verify(const Circuit* circuit) const {
    if(qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex);
    }
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledPhaseGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::PhaseGate::verify(circuit);
}