template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::PrintGate::PrintGate(const size_t &qubitIndex, std::ostream *outputStream) :
        SingleTargetGate(qubitIndex),
        outputStream(outputStream){}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::PrintGate::apply(Circuit<FloatingNumberType> *circuit) {
    *outputStream << circuit->qubits[SingleTargetGate::qubitIndex].getState() << std::endl;
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::PrintGate::verify(const Circuit* circuit) const {
    SingleTargetGate::verify(circuit);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::PrintGate::clone() const {
    return std::make_unique<PrintGate>(*this);
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::PrintGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    return Circuit<FloatingNumberType>::Gate::getStandardDrawing(circuit, "PRINT", SingleTargetGate::qubitIndex);
}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::PrintGate::getRepresentation() const {
    return "PRINT[Q#" + std::to_string(SingleTargetGate::qubitIndex) + "]";
}