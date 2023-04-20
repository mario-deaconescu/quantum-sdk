template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine,
                                     const size_t &qubitCount, const size_t &classicBitCount):
                                        probabilityEngine(probabilityEngine),
                                        qubits(std::vector<Qubit<FloatingNumberType>>(qubitCount, Qubit<FloatingNumberType>(probabilityEngine))),
                                        classicBits(std::vector<ClassicBit>(classicBitCount)) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine,
                                     const size_t &qubitCount):
        probabilityEngine(probabilityEngine),
        qubits(std::vector<Qubit<FloatingNumberType>>(qubitCount, Qubit<FloatingNumberType>(probabilityEngine))) {}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::Gate::getStandardDrawing(const Circuit<FloatingNumberType>* circuit, const std::string& identifier, const size_t& qubitIndex){
    std::array<std::string, 3> drawing = {"    ", "────", "    "};
    std::array<std::string, 3> targetDrawing = {"┌──┐", "┤  ├", "└──┘"};
    std::array<std::string, 3> measureDrawing = {"    ", "════", "    "};
    targetDrawing[1].insert(targetDrawing[1].length() / 2, identifier);
    for(size_t i = 0; i < identifier.length(); i++){
        drawing[0].insert(1, " ");
        drawing[1].insert(std::string("─").length(), "─");
        drawing[2].insert(1, " ");
        targetDrawing[0].insert(std::string("─").length(), "─");
        targetDrawing[2].insert(std::string("─").length(), "─");
        measureDrawing[0].insert(1, " ");
        measureDrawing[1].insert(std::string("═").length(), "═");
        measureDrawing[2].insert(1, " ");
    }
    std::vector<std::array<std::string, 3>> drawings(circuit->qubits.size() + 1, drawing);
    drawings[qubitIndex] = targetDrawing;
    drawings[circuit->qubits.size()] = measureDrawing;
    return drawings;
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Gate::Drawings Circuit<FloatingNumberType>::ControlledGate::getStandardDrawing(const Circuit<FloatingNumberType>* circuit, const std::string& identifier, const size_t& qubitIndex) const{
    const bool controlBeforeTarget = controlIndex < qubitIndex;
    std::array<std::string, 3> outsideDrawing = {"    ", "────", "    "};
    std::array<std::string, 3> insideDrawing = {" │  ", "─┼──", " │  "};
    std::array<std::string, 3> controlDrawing = {"    ", "─▉──", "    "};
    controlDrawing[controlBeforeTarget ? 2 : 0] = " │  ";
    std::array<std::string, 3> targetDrawing = {"┌──┐", "┤  ├", "└──┘"};
    if(controlBeforeTarget){
        targetDrawing[0] = "┌┴─┐";
    } else {
        targetDrawing[2] = "└┬─┘";
    }
    std::array<std::string, 3> measureDrawing = {"    ", "════", "    "};
    targetDrawing[1].insert(targetDrawing[1].length() / 2, identifier);
    for(size_t i = 0; i < identifier.length(); i++){
        const auto insertionLambda = [i](const std::string& string, const std::string& singleCharacter){
            return i % 2 == 0 ? singleCharacter.length() : string.length() - singleCharacter.length();
        };
        outsideDrawing[0].insert(insertionLambda(outsideDrawing[0], " "), " ");
        outsideDrawing[1].insert(insertionLambda(outsideDrawing[1], "─"), "─");
        outsideDrawing[2].insert(insertionLambda(outsideDrawing[2], " "), " ");
        insideDrawing[0].insert(insertionLambda(insideDrawing[0], " "), " ");
        insideDrawing[1].insert(insertionLambda(insideDrawing[1], "─"), "─");
        insideDrawing[2].insert(insertionLambda(insideDrawing[2], " "), " ");
        controlDrawing[0].insert(insertionLambda(controlDrawing[0], " "), " ");
        controlDrawing[1].insert(insertionLambda(controlDrawing[1], "─"), "─");
        controlDrawing[2].insert(insertionLambda(controlDrawing[2], " "), " ");
        targetDrawing[0].insert(insertionLambda(targetDrawing[0], "─"), "─");
        targetDrawing[2].insert(insertionLambda(targetDrawing[2], "─"), "─");
        measureDrawing[0].insert(insertionLambda(measureDrawing[0], " "), " ");
        measureDrawing[1].insert(insertionLambda(measureDrawing[1], "═"), "═");
        measureDrawing[2].insert(insertionLambda(measureDrawing[2], " "), " ");
    }
    std::vector<std::array<std::string, 3>> drawings(circuit->qubits.size() + 1);
    size_t i;
    for(i = 0; i < std::min(controlIndex, qubitIndex); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[i] = controlBeforeTarget ? controlDrawing : targetDrawing;
    for(i++; i < std::max(controlIndex, qubitIndex); i++){
        drawings[i] = insideDrawing;
    }
    drawings[i] = controlBeforeTarget ? targetDrawing : controlDrawing;
    for(i++; i < circuit->qubits.size(); i++){
        drawings[i] = outsideDrawing;
    }
    drawings[i] = measureDrawing;
    return drawings;
}

template<std_floating_point FloatingNumberType>
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

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::Result::Result(const std::vector<ClassicBit> &classicBits): classicBits(classicBits) {}

template<std_floating_point FloatingNumberType>
std::string Circuit<FloatingNumberType>::Result::getRepresentation() const {
    std::string representation;
    for(auto& classicBit : classicBits){
        representation += classicBit.getRepresentation();
    }
    std::reverse(representation.begin(), representation.end());
    return representation;
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::CompoundResult::CompoundResult() = default;

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::CompoundResult::CompoundResult(const std::vector<Result> &results) {
    for (const auto& result : results){
        addResult(result);
    }
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::CompoundResult::addResult(const Circuit::Result &result) {
    const auto& representation = result.getRepresentation();
    if(resultMap.find(representation) == resultMap.end()){
        resultMap[representation] = 1;
    } else {
        resultMap[representation]++; 
    }
}

template<std_floating_point FloatingNumberType>
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

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addGate(std::unique_ptr<Gate> gate){
    // Check if the gate is a CircuitGate
    auto* circuitGate = dynamic_cast<CircuitGate*>(gate.get());
    if(circuitGate != nullptr){
        Circuit<FloatingNumberType>::CircuitGate::issueUninitializedCircuitGateWarning();
        std::vector<size_t> qubitIndices(circuitGate->getQubitCount());
        for(size_t i = 0; i < qubitIndices.size(); i++){
            qubitIndices[i] = i;
        }
        circuitGate->setQubitIndices(qubitIndices);
    }
    gate->verify(this);
    // Transfer ownership of the gate to the circuitPointer
    gates.emplace_back(std::move(gate));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addGate(std::unique_ptr<CircuitGate> gate, const std::vector<size_t>& qubitIndices){
    gate->setQubitIndices(qubitIndices);
    gate->verify(this);
    // Transfer ownership of the gate to the circuitPointer
    gates.emplace_back(std::move(gate));
}

template<std_floating_point FloatingNumberType>
bool Circuit<FloatingNumberType>::ControlledGate::getControlState(Circuit<FloatingNumberType> *circuit) const {
    auto& controlQubit = circuit->qubits[controlIndex];
    return controlQubit.measure().getState() == ClassicBit::State::ONE;
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::ControlledGate::verify(const Circuit* circuit) const {
    if(controlIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(controlIndex);
    }
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::ControlledGate::ControlledGate(const size_t& controlIndex): controlIndex(controlIndex) {}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::Result Circuit<FloatingNumberType>::run() {
    for(auto& gate : gates){
        gate->apply(this);
    }
    return Result(classicBits);
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addMeasureGate(const std::vector<std::pair<size_t, size_t>> &qubitClassicBitPairs) {
    addGate(std::make_unique<MeasureGate>(qubitClassicBitPairs));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addHadamardGate(const size_t &qubitIndex) {
    addGate(std::make_unique<HadamardGate>(qubitIndex));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addControlledHadamardGate(const size_t &controlQubitIndex,
                                                            const size_t &targetQubitIndex) {
    addGate(static_cast<std::unique_ptr<HadamardGate>>(std::make_unique<ControlledHadamardGate>(controlQubitIndex, targetQubitIndex)));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addXGate(const size_t &qubitIndex) {
    addGate(std::make_unique<XGate>(qubitIndex));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addCXGate(const size_t &controlQubitIndex, const size_t &targetQubitIndex) {
    addGate(static_cast<std::unique_ptr<XGate>>(std::make_unique<CXGate>(controlQubitIndex, targetQubitIndex)));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addYGate(const size_t &qubitIndex) {
    addGate(std::make_unique<YGate>(qubitIndex));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addCYGate(const size_t &controlQubitIndex, const size_t &targetQubitIndex) {
    addGate(static_cast<std::unique_ptr<YGate>>(std::make_unique<CYGate>(controlQubitIndex, targetQubitIndex)));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addZGate(const size_t &qubitIndex) {
    addGate(std::make_unique<ZGate>(qubitIndex));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addCZGate(const size_t &controlQubitIndex, const size_t &targetQubitIndex) {
    addGate(static_cast<std::unique_ptr<ZGate>>(std::make_unique<CZGate>(controlQubitIndex, targetQubitIndex)));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addSwapGate(const size_t &qubitIndex1, const size_t &qubitIndex2) {
    addGate(std::make_unique<SwapGate>(qubitIndex1, qubitIndex2));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addCircuitGate(const Circuit<FloatingNumberType>& circuit, const std::vector<size_t> &qubitIndices) {
    addGate(std::make_unique<CircuitGate>(circuit, qubitIndices));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addPhaseGate(const size_t &qubitIndex, const FloatingNumberType &angle) {
    addGate(std::make_unique<PhaseGate>(qubitIndex, angle));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addControlledPhaseGate(const size_t &controlQubitIndex,
                                                         const size_t &targetQubitIndex,
                                                         const FloatingNumberType &angle) {
    addGate(static_cast<std::unique_ptr<PhaseGate>>(std::make_unique<ControlledPhaseGate>(controlQubitIndex, targetQubitIndex, angle)));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addInitGate(const size_t &qubitIndex,
                                              const typename Qubit<FloatingNumberType>::State &state) {
    addGate(std::make_unique<InitGate>(qubitIndex, state));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::addPrintGate(const size_t &qubitIndex) {
    addGate(std::make_unique<PrintGate>(qubitIndex));
}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::reset() {
    for(auto& qubit : qubits){
        qubit = Qubit<FloatingNumberType>(probabilityEngine);
    }
    for(auto& classicBit : classicBits){
        classicBit = ClassicBit();
    }
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::CompoundResult Circuit<FloatingNumberType>::simulate(const size_t &count) {
    CompoundResult result;
    for(size_t i = 0; i < count; i++){
        reset();
        result.addResult(run());
    }
    return result;
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::InvalidQubitIndexException::InvalidQubitIndexException(const size_t &qubitIndex):
std::runtime_error("Invalid qubit index: " + std::to_string(qubitIndex)),
index(qubitIndex)  {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::InvalidClassicBitIndexException::InvalidClassicBitIndexException(const size_t &classicIndex):
std::runtime_error("Invalid classic bit index: " + std::to_string(classicIndex)),
index(classicIndex) {}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::Circuit(const Circuit &other): Circuit(other.probabilityEngine, other.qubits.size(), other.classicBits.size()) {
    for(const auto& gate : other.gates){
        addGate(gate->clone());
    }
}

template<std_floating_point FloatingNumberType>
typename Circuit<FloatingNumberType>::CircuitGate Circuit<FloatingNumberType>::toGate() const {
    return CircuitGate(*this);
}

template<std_floating_point FloatingNumberType>
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::Gate::makeControlled(const size_t& controlIndex, const bool& classic) const {
    return std::make_unique<CustomControlledGate>(controlIndex, clone(), classic);
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>& Circuit<FloatingNumberType>::operator+=(const Circuit &other) {
    // TODO check if other has the same number of qubits and classic bits
    for(const auto& gate : other.gates){
        addGate(gate->clone());
    }
    return *this;
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType> &Circuit<FloatingNumberType>::operator=(const Circuit &other) {
    if(this != &other){
        Circuit<FloatingNumberType> temp(other);
        std::swap(temp.probabilityEngine, probabilityEngine);
        std::swap(temp.qubits, qubits);
        std::swap(temp.classicBits, classicBits);
        std::swap(temp.gates, gates);
    }
    return *this;
}

template<std_floating_point FloatingNumberType>
size_t Circuit<FloatingNumberType>::getQubitCount() const {
    return qubits.size();
}

template<std_floating_point FloatingNumberType>
size_t Circuit<FloatingNumberType>::getClassicBitCount() const {
    return classicBits.size();
}

template<std_floating_point FloatingNumberType>
const std::vector<std::unique_ptr<typename Circuit<FloatingNumberType>::Gate>> &Circuit<FloatingNumberType>::getGates() const{
    return gates;
}

template<std_floating_point FloatingNumberType>
size_t Circuit<FloatingNumberType>::ControlledGate::getControlIndex() const {
    return controlIndex;
}