template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::InitGate::InitGate(const size_t &qubitIndex,
                                                const typename Qubit<FloatingNumberType>::State &state):
        SingleTargetGate(qubitIndex),
        state(state) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::InitGate::InitGate(const Circuit::InitGate &other):
        SingleTargetGate(other.qubitIndex),
        state(other.state) {}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::InitGate::apply(Circuit<FloatingNumberType> *circuit) {
    circuit->qubits[SingleTargetGate::qubitIndex].setState(state);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::InitGate::verify(const Circuit *circuit) const {
    SingleTargetGate::verify(circuit);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::InitGate::clone() const {
    return std::make_unique<InitGate>(*this);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::InitGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "INIT", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::InitGate::getRepresentation() const {
    return "INIT[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}