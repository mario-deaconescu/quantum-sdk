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
std::unique_ptr<typename Circuit<FloatingNumberType>::Gate> Circuit<FloatingNumberType>::MeasureGate::clone() const {
    return std::make_unique<MeasureGate>(*this);
}

template<typename FloatingNumberType>
void Circuit<FloatingNumberType>::MeasureGate::assertValid(const Circuit* circuit) const {
    for(auto& [qubitIndex, classicBitIndex] : qubitClassicBitPairs){
        if(qubitIndex >= circuit->qubits.size()){
            throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex);
        } else if(classicBitIndex >= circuit->classicBits.size()){
            throw Circuit<FloatingNumberType>::InvalidClassicBitIndexException(classicBitIndex);
        }
    }
}