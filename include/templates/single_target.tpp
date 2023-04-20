#pragma once

template<std_floating_point FloatingNumberType>
size_t Circuit<FloatingNumberType>::SingleTargetGate::getTargetIndex() const {
    return qubitIndex;
}

template<std_floating_point FloatingNumberType>
Circuit<FloatingNumberType>::SingleTargetGate::SingleTargetGate(const size_t &targetIndex): qubitIndex(targetIndex) {}

template<std_floating_point FloatingNumberType>
void Circuit<FloatingNumberType>::SingleTargetGate::verify(const Circuit *circuit) const {
    if(qubitIndex >= circuit->qubits.size()){
        throw Circuit<FloatingNumberType>::InvalidQubitIndexException(qubitIndex);
    }
}