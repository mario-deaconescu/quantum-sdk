#include <utility>


template<typename FloatingNumberType>
[[deprecated]]
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CircuitGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    typename Gate::Drawings drawings(circuit->qubits.size() + 1);
    const size_t minQubitIndex = *std::min_element(qubitIndices.begin(), qubitIndices.end());
    const size_t maxQubitIndex = *std::max_element(qubitIndices.begin(), qubitIndices.end());
    const size_t maxIndexLength = std::to_string(circuit->qubits.size() - 1).length();
    const size_t gateHeight = maxQubitIndex - minQubitIndex + 1;
    const size_t gateWidth = 4 + maxIndexLength + name.length();
    std::array<std::string, 3> outsideDrawing = {std::string(gateWidth, ' '), "", std::string(gateWidth, ' ')};
    std::array<std::string, 3> measureDrawing = {std::string(gateWidth, ' '), "", std::string(gateWidth, ' ')};
    std::map<size_t, std::string> qubitIndexToGateIndex;
    for(size_t i = 0; i < qubitIndices.size(); i++){
        qubitIndexToGateIndex[qubitIndices[i]] = std::to_string(i);
    }
    for(size_t i = 0; i < gateWidth; i++){
        outsideDrawing[1] += "─";
        measureDrawing[1] += "═";
    }
    for(size_t i = 0; i < minQubitIndex; i++){
        drawings[i] = outsideDrawing;
    }
    for(size_t i = minQubitIndex, qubitCount = 0; i <= maxQubitIndex; i++, qubitCount++){
        drawings[i][0] = "│";
        drawings[i][0] += std::string(gateWidth - 2, ' ');
        drawings[i][0] += "│";
        drawings[i][1] = "┤";
        const std::string qubitCountString = qubitIndexToGateIndex.find(i) != qubitIndexToGateIndex.end() ? qubitIndexToGateIndex[i] : "";
        drawings[i][1] += qubitCountString;
        drawings[i][1] += std::string(gateWidth - 2 - qubitCountString.length(), ' ');
        drawings[i][1] += "├";
        drawings[i][2] = "│";
        drawings[i][2] += std::string(gateWidth - 2, ' ');
        drawings[i][2] += "│";
    }
    drawings[minQubitIndex][0] = "┌";
    for(size_t i = 1; i < gateWidth - 1; i++){
        drawings[minQubitIndex][0] += "─";
    }
    drawings[minQubitIndex][0] += "┐";
    drawings[maxQubitIndex][2] = "└";
    for(size_t i = 1; i < gateWidth - 1; i++){
        drawings[maxQubitIndex][2] += "─";
    }
    drawings[maxQubitIndex][2] += "┘";
    // Insert name in the middle
    const size_t middleIndex = (gateHeight - 1) / 2 + minQubitIndex;
    const size_t drawingRow = gateHeight % 2 == 1 ? 1 : 2;
    for(size_t drawingColumn = std::to_string(minQubitIndex - minQubitIndex).length() + 1 + std::string("┤").length(),
            nameColumn = 0; nameColumn < name.length(); drawingColumn++, nameColumn++) {
        drawings[middleIndex][drawingRow][drawingColumn] = name[nameColumn];
    }
    for(size_t i = maxQubitIndex + 1; i < circuit->qubits.size(); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[circuit->qubits.size()] = measureDrawing;
    return drawings;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CircuitGate::CircuitGate(const Circuit<FloatingNumberType> &circuit): circuitPointer(
        std::make_shared<Circuit>(circuit)) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CircuitGate::CircuitGate(const Circuit<FloatingNumberType> &circuit,
                                                      std::string name): circuitPointer(
        std::make_shared<Circuit>(circuit)), name(std::move(name)) {}


template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CircuitGate::CircuitGate(std::shared_ptr<Circuit> circuit): circuitPointer(circuit) {}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CircuitGate::CircuitGate(std::shared_ptr<Circuit<FloatingNumberType>> circuit,
                                                      const std::vector<size_t> &indices): circuitPointer(circuit),
                                                                                           qubitIndices(indices) {}

template<typename FloatingNumberType>
[[deprecated]]
std::string Circuit<FloatingNumberType>::CircuitGate::getRepresentation() const {
    return "";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CircuitGate::apply(Circuit<FloatingNumberType> *circuit) {
    for (size_t i = 0; i < circuitPointer->qubits.size(); i++) {
        circuitPointer->qubits[i].setState(circuit->qubits[qubitIndices[i]].getState());
    }
    circuitPointer->run();
    for (size_t i = 0; i < circuitPointer->qubits.size(); i++) {
        circuit->qubits[qubitIndices[i]].setState(circuitPointer->qubits[i].getState());
    }
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CircuitGate::InvalidQubitIndicesException::InvalidQubitIndicesException(
        const Circuit::CircuitGate &gate, const std::vector<size_t> &indices):
        qubitCount(gate.getQubitCount()),
        indexCount(indices.size()),
        message(std::string("Invalid qubit indices for CircuitGate: " + std::to_string(indexCount) +
                            " indices were provided, but the CircuitGate has " + std::to_string(qubitCount) +
                            " qubits.")) {}

template<typename FloatingNumberType>
const char *Circuit<FloatingNumberType>::CircuitGate::InvalidQubitIndicesException::what() const noexcept {
    return message.c_str();
}

template<typename FloatingNumberType>
size_t Circuit<FloatingNumberType>::CircuitGate::getQubitCount() const {
    return circuitPointer->qubits.size();
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CircuitGate::setQubitIndices(const std::vector<size_t> &indices) {
    if (indices.size() != getQubitCount()) {
        throw InvalidQubitIndicesException(*this, indices);
    }
    this->qubitIndices = indices;
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CircuitGate::issueUninitializedCircuitGateWarning() {
    std::cerr << "WARNING: CircuitGate was added to a circuit without the qubit indices being set. "
                 "To add a CircuitGate to a Circuit, use Circuit.addGate(circuitGate, qubitIndices) or Circuit.addCircuitGate(circuit, qubitIndices)."
              << std::endl;
}

template<typename FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::CircuitGate::clone() const {
    return std::make_unique<CircuitGate>(*this);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CircuitGate::verify(const Circuit *circuit) const {
    for (unsigned long qubitIndex: qubitIndices) {
        if (qubitIndex >= circuit->qubits.size()) {
            throw InvalidQubitIndexException(qubitIndex);
        }
    }
}