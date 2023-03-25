template<typename FloatingNumberType>
Circuit<FloatingNumberType>::SwapGate::SwapSameQubitException::SwapSameQubitException(const size_t &qubitIndex):
                                            qubitIndex(qubitIndex),
                                            message("Cannot swap qubit with itself: " + std::to_string(qubitIndex)) {}

template<typename FloatingNumberType>
const char *Circuit<FloatingNumberType>::SwapGate::SwapSameQubitException::what() const noexcept {
    return message.c_str();
}

template<typename FloatingNumberType>
[[deprecated]]
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::SwapGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    typename Gate::Drawings drawings(circuit->qubits.size() + 1);
    std::array<std::string, 3> outsideDrawing = {"       ", "───────", "       "};
    std::array<std::string, 3> insideDrawing = {"   │   ", "───┼───", "   │   "};
    std::array<std::string, 3> measureDrawing = {"       ", "═══════", "       "};
    std::array<std::string, 3> topTargetDrawing = {"┌─────┐", "┤ SWP ├", "└──┬──┘"};
    std::array<std::string, 3> bottomTargetDrawing = {"┌──┴──┐", "┤ SWP ├", "└─────┘"};
    size_t minQubitIndex;
    size_t maxQubitIndex;
    if(qubitIndex1 < qubitIndex2){
        minQubitIndex = qubitIndex1;
        maxQubitIndex = qubitIndex2;
    } else {
        minQubitIndex = qubitIndex2;
        maxQubitIndex = qubitIndex1;
    }
    for(size_t i = 0; i < minQubitIndex; i++){
        drawings[i] = outsideDrawing;
    }
    drawings[minQubitIndex] = topTargetDrawing;
    for(size_t i = minQubitIndex + 1; i < maxQubitIndex; i++){
        drawings[i] = insideDrawing;
    }
    drawings[maxQubitIndex] = bottomTargetDrawing;
    for(size_t i = maxQubitIndex + 1; i < circuit->qubits.size(); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[circuit->qubits.size()] = measureDrawing;
    return drawings;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::SwapGate::SwapGate(const size_t &qubitIndex1, const size_t &qubitIndex2): qubitIndex1(qubitIndex1), qubitIndex2(qubitIndex2) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::SwapGate::getRepresentation() const {
    return "SWAP[Q#" + std::to_string(qubitIndex1) + " ↔ Q#" + std::to_string(qubitIndex2) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::SwapGate::apply(Circuit<FloatingNumberType> *circuit) {
    // Swap qubits
    std::swap(circuit->qubits[qubitIndex1], circuit->qubits[qubitIndex2]);
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::SwapGate::clone() const {
    return std::make_unique<SwapGate>(*this);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::SwapGate::verify(const Circuit* circuit) const {
    if(qubitIndex1 == qubitIndex2){
        throw SwapSameQubitException(qubitIndex1);
    }
    if(qubitIndex1 >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex1);
    }
    if(qubitIndex2 >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex2);
    }
}