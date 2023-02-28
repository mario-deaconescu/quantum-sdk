template<typename FloatingNumberType>
Circuit<FloatingNumberType>::Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine,
                                     const size_t &qubitCount, const size_t &classicBitCount):
                                        probabilityEngine(probabilityEngine),
                                        qubits(std::vector<Qubit<FloatingNumberType>>(qubitCount, Qubit<FloatingNumberType>(probabilityEngine))),
                                        classicBits(std::vector<ClassicBit>(classicBitCount)) {}



template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::getRepresentation() const {
    std::vector<std::array<std::string, 3>> drawings(qubits.size() + 1, {"", "", ""});
    size_t maxQubitNameLength = std::to_string(qubits.size() - 1).length();
    for(size_t i = 0; i < qubits.size(); i++){
        for(size_t j = 0; j < maxQubitNameLength + 4; j++) {
            drawings[i][0] += " ";
            drawings[i][2] += " ";
        }
        std::string qubitName = std::to_string(i);
        for(size_t j = 0; j < maxQubitNameLength - qubitName.length(); j++){
            drawings[i][1] += " ";
        }
        drawings[i][1] += "Q#" + qubitName + " >";
    }
    for(size_t i = 0; i < maxQubitNameLength + 1; i++) {
        drawings[qubits.size()][0] += " ";
        drawings[qubits.size()][1] += " ";
        drawings[qubits.size()][2] += " ";
    }
    drawings[qubits.size()][0] += "   ";
    drawings[qubits.size()][1] += "C >";
    drawings[qubits.size()][2] += "   ";
    for(const auto& gate : gates){
        const auto& gateDrawings = gate->getDrawings(this);
        for(size_t i = 0; i < gateDrawings.size(); i++){
            drawings[i][0] += gateDrawings[i][0];
            drawings[i][1] += gateDrawings[i][1];
            drawings[i][2] += gateDrawings[i][2];
        }
    }
    std::string representation;
    for(const auto& drawing : drawings) {
        representation += drawing[0] + "\n" + drawing[1] + "\n" + drawing[2] + "\n";
    }
    return representation;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::Result::Result(const std::vector<ClassicBit> &classicBits): classicBits(classicBits) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::Result::getRepresentation() const {
    std::string representation;
    for(auto& classicBit : classicBits){
        representation += classicBit.getRepresentation();
    }
    return representation;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CompoundResult::CompoundResult() = default;

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CompoundResult::CompoundResult(const std::vector<Result> &results) {
    for (const auto& result : results){
        addResult(result);
    }
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CompoundResult::addResult(const Circuit::Result &result) {
    const auto& representation = result.getRepresentation();
    if(resultMap.find(representation) == resultMap.end()){
        resultMap[representation] = 1;
    } else {
        resultMap[representation]++; 
    }
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::CompoundResult::getRepresentation() const {
    std::string representation = "{\n";
    for (const auto& [key, value] : resultMap){
        representation += "\t❘" + key + "〉 : " + std::to_string(value) + ",\n";
    }
    representation.pop_back();
    representation.pop_back();
    representation += "\n}";
    return representation;
}


template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::addGate(std::unique_ptr<Gate> gate){
    // Transfer ownership of the gate to the circuit
    gates.emplace_back(std::move(gate));
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::MeasureGate::MeasureGate(const std::vector<std::pair<size_t, size_t>>& qubitClassicBitPairs) : qubitClassicBitPairs(qubitClassicBitPairs) {}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::MeasureGate::apply(Circuit<FloatingNumberType> *circuit) {
    for (auto& qubitClassicBitPair : qubitClassicBitPairs){
        auto& qubit = circuit->qubits[qubitClassicBitPair.first];
        auto& classicBit = circuit->classicBits[qubitClassicBitPair.second];
        classicBit = qubit.measure();
    }
}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::MeasureGate::getRepresentation() const {
    std::string representation = "M[";
    for(size_t i = 0; i < qubitClassicBitPairs.size(); i++){
        representation += "Q#" + std::to_string(qubitClassicBitPairs[i].first) + " → C#" + std::to_string(qubitClassicBitPairs[i].second);
        if(i != qubitClassicBitPairs.size() - 1){
            representation += ", ";
        }
    }
    representation += "]";
    return representation;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::HadamardGate::HadamardGate(const size_t &qubitIndex): qubitIndex(qubitIndex) {}

template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::HadamardGate::getRepresentation() const {
    return "H[Q#" + std::to_string(qubitIndex) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::HadamardGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& qubit = circuit->qubits[qubitIndex];
    const auto alpha = qubit.getState().getAlpha();
    const auto beta = qubit.getState().getBeta();
    const auto newAlpha = (alpha + beta) / (FloatingNumberType)std::sqrt(2);
    const auto newBeta = (alpha - beta) / (FloatingNumberType)std::sqrt(2);
    qubit.setState(newAlpha, newBeta);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Result Circuit<FloatingNumberType>::run() {
    for(auto& gate : gates){
        gate->apply(this);
    }
    return Result(classicBits);
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::HadamardGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    std::array<std::string, 3> drawing = {"     ", "─────", "     "};
    std::array<std::string, 3> targetDrawing = {"┌───┐", "┤ H ├", "└───┘"};
    std::array<std::string, 3> measureDrawing = {"     ", "═════", "     "};
    std::vector<std::array<std::string, 3>> drawings(circuit->qubits.size() + 1, drawing);
    drawings[qubitIndex] = targetDrawing;
    drawings[circuit->qubits.size()] = measureDrawing;
    return drawings;
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::CNOTGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    const bool controlBeforeTarget = controlQubitIndex < targetQubitIndex;
    std::array<std::string, 3> outsideDrawing = {"     ", "─────", "     "};
    std::array<std::string, 3> insideDrawing = {"  │  ", "──┼──", "  │  "};
    std::array<std::string, 3> controlDrawing = {"     ", "──▉──", "     "};
    controlDrawing[controlBeforeTarget ? 2 : 0] = "  │  ";
    std::array<std::string, 3> targetDrawing = {"┌───┐", "┤ X ├", "└───┘"};
    if(controlBeforeTarget){
        targetDrawing[0] = "┌─┴─┐";
    } else {
        targetDrawing[2] = "└─┬─┘";
    }
    std::array<std::string, 3> measureDrawing = {"     ", "═════", "     "};
    std::vector<std::array<std::string, 3>> drawings(circuit->qubits.size() + 1);
    size_t i;
    for(i = 0; i < std::min(controlQubitIndex, targetQubitIndex); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[i] = controlBeforeTarget ? controlDrawing : targetDrawing;
    for(i++; i < std::max(controlQubitIndex, targetQubitIndex); i++){
        drawings[i] = insideDrawing;
    }
    drawings[i] = controlBeforeTarget ? targetDrawing : controlDrawing;
    for(i++; i < circuit->qubits.size(); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[i] = measureDrawing;
    return drawings;
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::MeasureGate::getDrawings(
        const Circuit<FloatingNumberType> *circuit) const {
    std::vector<std::array<std::string, 3>> drawings(circuit->qubits.size() + 1);
    for(auto& [qubitIndex, classicBitIndex] : qubitClassicBitPairs){
        std::string indexString = std::to_string(classicBitIndex);
        size_t indexStringLength = indexString.size() % 2 == 0 ? indexString.size() + 1 : indexString.size();
        std::string emptyRow = "  ";
        std::string verticalRowOutside = " ";
        std::string verticalRowInside = "─";
        std::string lineRow = "──";
        std::string boxTop = "┌";
        std::string boxMiddle = "┤";
        std::string boxBottom = "└";
        std::string measureTop = " ";
        std::string measureMiddle = "═";
        std::string measureBottom = (indexStringLength > indexString.length() ? "  " : " " ) + indexString + " ";
        for(size_t i = 0; i < indexStringLength; i++) {
            if(i == indexStringLength / 2) {
                boxMiddle += "M";
                boxBottom += "╦";
                measureTop += "║";
                measureMiddle += "╩";
                verticalRowOutside += "║";
                verticalRowInside += "╫";
            } else {
                boxMiddle += " ";
                boxBottom += "─";
            }
            emptyRow += " ";
            lineRow += "─";
            boxTop += "─";
        }
        boxTop += "┐";
        boxMiddle += "├";
        boxBottom += "┘";
        verticalRowOutside += " ";
        verticalRowInside += "─";
        measureTop += " ";
        measureMiddle += "═";
        std::array<std::string, 3> insideDrawing = {verticalRowOutside, verticalRowInside, verticalRowOutside};
        std::array<std::string, 3> outsideDrawing = {emptyRow, lineRow, emptyRow};
        std::array<std::string, 3> targetDrawing = {boxTop, boxMiddle, boxBottom};
        std::array<std::string, 3> measureDrawing = {measureTop, measureMiddle, measureBottom};
        size_t i;
        for(i = 0; i < qubitIndex; i++){
            for(size_t j = 0; j < 3; j++){
                drawings[i][j] += outsideDrawing[j];
            }
        }
        for(size_t j = 0; j < 3; j++){
            drawings[i][j] += targetDrawing[j];
        }
        for(i++; i < drawings.size() - 1; i++){
            for(size_t j = 0; j < 3; j++){
                drawings[i][j] += insideDrawing[j];
            }
        }
        for(size_t j = 0; j < 3; j++){
            drawings[i][j] += measureDrawing[j];
        }
    }
    return drawings;
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::addMeasureGate(const std::vector<std::pair<size_t, size_t>> &qubitClassicBitPairs) {
    addGate(std::make_unique<MeasureGate>(qubitClassicBitPairs));
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::addHadamardGate(const size_t &qubitIndex) {
    addGate(std::make_unique<HadamardGate>(qubitIndex));
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::addCNOTGate(const size_t &controlQubitIndex, const size_t &targetQubitIndex) {
    addGate(std::make_unique<CNOTGate>(controlQubitIndex, targetQubitIndex));
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::reset() {
    for(auto& qubit : qubits){
        qubit = Qubit<FloatingNumberType>(probabilityEngine);
    }
    for(auto& classicBit : classicBits){
        classicBit = ClassicBit();
    }
}

template<typename FloatingNumberType>
typename Circuit<FloatingNumberType>::CompoundResult Circuit<FloatingNumberType>::simulate(const size_t &count) {
    CompoundResult result;
    for(size_t i = 0; i < count; i++){
        reset();
        result.addResult(run());
    }
    return result;
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::CNOTGate::CNOTGate(const size_t &controlQubitIndex, const size_t &targetQubitIndex):
        controlQubitIndex(controlQubitIndex), targetQubitIndex(targetQubitIndex) {}


template<typename FloatingNumberType>
std::string Circuit<FloatingNumberType>::CNOTGate::getRepresentation() const {
    return "CX[Q#" + std::to_string(controlQubitIndex) + " ⇏ Q#" + std::to_string(targetQubitIndex) + "]";
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::CNOTGate::apply(Circuit<FloatingNumberType> *circuit) {
    auto& controlQubit = circuit->qubits[controlQubitIndex];
    auto& targetQubit = circuit->qubits[targetQubitIndex];
    if(controlQubit.measure().getState() == ClassicBit::State::ONE){
        const auto newAlpha = targetQubit.getState().getBeta();
        const auto newBeta = targetQubit.getState().getAlpha();
        targetQubit.setState(newAlpha, newBeta);
    }
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::InvalidQubitIndexException::InvalidQubitIndexException(const size_t &qubitIndex): index(qubitIndex)  {
    message = "Invalid qubit index: " + std::to_string(index);
}

template<typename FloatingNumberType>
const char *Circuit<FloatingNumberType>::InvalidQubitIndexException::what() const noexcept {
    return message.c_str();
}

template<typename FloatingNumberType>
Circuit<FloatingNumberType>::InvalidClassicBitIndexException::InvalidClassicBitIndexException(const size_t &classicIndex): index(classicIndex) {}

template<typename FloatingNumberType>
const char *Circuit<FloatingNumberType>::InvalidClassicBitIndexException::what() const noexcept {
    return message.c_str();
}


