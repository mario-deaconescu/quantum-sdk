template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings
Circuit<FloatingNumberType>::CustomControlledGate::getDrawings(const Circuit<FloatingNumberType> *circuit) const {
    typename Gate::Drawings drawings = gatePointer->getDrawings(circuit);
    const size_t drawingWidth = drawings[controlIndex][1].length();
    const size_t lineLength = std::string("─").length();
    const size_t insertIndex = drawingWidth / lineLength / 2 * lineLength;
    const std::string replacementString = std::string("▓");
    drawings[controlIndex][1].erase(insertIndex, lineLength);
    drawings[controlIndex][1].insert(insertIndex, "▓");
    return drawings;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CustomControlledGate::CustomControlledGate(const size_t &controlQubitIndex,
                                                                        std::unique_ptr<Gate> gate):
                                                                        controlIndex(controlQubitIndex),
                                                                        gatePointer(gate->clone()){}


template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CustomControlledGate::CustomControlledGate(const Circuit::CustomControlledGate &other):
                                                                        controlIndex(other.controlIndex),
                                                                        gatePointer(other.gatePointer->clone()){}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::CustomControlledGate::getRepresentation() const {
    return "C:" + gatePointer->getRepresentation();
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CustomControlledGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& controlQubit = circuit->qubits[controlIndex];
    if(controlQubit.measure().getState() == ClassicBit::State::ONE){
        gatePointer->apply(circuit);
    }
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CustomControlledGate::assertValid(const Circuit* circuit) const {
    if(controlIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(controlIndex);
    }
    gatePointer->assertValid(circuit);
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CustomControlledGate::clone() const {
    return std::make_unique<Circuit<FloatingNumberType>::CustomControlledGate>(*this);
}