template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ControlledPhaseGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(circuit, "P", PhaseGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::PhaseGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "P", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::PhaseGate::PhaseGate(const size_t &qubitIndex, const double& angle): SingleTargetGate(qubitIndex), angle(angle) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::ControlledPhaseGate::ControlledPhaseGate(const size_t &controlQubitIndex, const size_t &qubitIndex, const double& angle):
Circuit<FloatingNumberType>::SingleTargetGate(qubitIndex),
Circuit<FloatingNumberType>::ControlledGate(controlQubitIndex),
Circuit<FloatingNumberType>::PhaseGate(qubitIndex, angle) {}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::PhaseGate::getRepresentation() const {
    return "P[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::ControlledPhaseGate::getRepresentation() const {
    return "CP[Q#" + std::to_string(Circuit<FloatingNumberType>::ControlledGate::controlIndex) + " ⇏ Q#" + std::to_string(Circuit<FloatingNumberType>::PhaseGate::qubitIndex) + "]";
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::PhaseGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& targetQubit = circuit->qubits[SingleTargetGate::qubitIndex];
    const auto newAlpha = targetQubit.getState().getAlpha();
    const auto newBeta = targetQubit.getState().getBeta() * std::exp(std::complex<FloatingNumberType>(0, angle));
    targetQubit.setState(newAlpha, newBeta);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledPhaseGate::apply(Circuit<FloatingNumberType> *circuit) {
    if(Circuit<FloatingNumberType>::ControlledGate::getControlState(circuit)) {
        Circuit<FloatingNumberType>::PhaseGate::apply(circuit);
    }
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::PhaseGate::clone() const {
    return std::make_unique<PhaseGate>(*this);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::ControlledPhaseGate::clone() const {
    return static_cast<std::unique_ptr<PhaseGate>>(std::make_unique<ControlledPhaseGate>(*this));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::PhaseGate::verify(const Circuit* circuit) const {
    if(SingleTargetGate::qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(SingleTargetGate::qubitIndex);
    }
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledPhaseGate::verify(const Circuit* circuit) const {
    Circuit<FloatingNumberType>::ControlledGate::verify(circuit);
    Circuit<FloatingNumberType>::PhaseGate::verify(circuit);
}