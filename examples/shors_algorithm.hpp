#include <memory>
#include <utility>
#include "../include/circuit.hpp"

#include <numbers>

using namespace QPP;

/// @brief U gate for N = 15
Circuit<double>::CircuitGate UGate(const size_t& a, const unsigned long& power){
    // Controlled multiplication by a mod 15

    // Check if a is not coprime to N (15)
    if(a != 2 && a != 4 && a != 7 && a != 8 && a != 11 && a != 13){
        throw std::invalid_argument("a must be coprime to N (15)");
    }
    auto circuit = Circuit<double>(std::make_shared<ProbabilityEngine<double>>(), 4);
    for (size_t iteration = 0; iteration < power; iteration++) {
        if(a == 2 || a == 13){
            circuit.addSwapGate(2, 3);
            circuit.addSwapGate(1, 2);
            circuit.addSwapGate(0, 1);
        } else if(a == 7 || a == 8){
            circuit.addSwapGate(0, 1);
            circuit.addSwapGate(1, 2);
            circuit.addSwapGate(2, 3);
        } else if(a == 4 || a == 11){
            circuit.addSwapGate(1, 3);
            circuit.addSwapGate(0, 2);
        }
        if(a == 7 || a == 11 || a == 13){
            for (size_t index = 0; index < 4; index++) {
                circuit.addXGate(index);
            }
        }
    }
    auto uGate = circuit.toGate();
    uGate.name = std::to_string(a) + "^" + std::to_string(power) + " mod 15";

    return uGate;
}

/// @brief Quantum Fourier Transform
Circuit<double> CQFT(const std::shared_ptr<ProbabilityEngine<double>>& probabilityEngine, const size_t &n) {
    Circuit<double> circuit(probabilityEngine, n, n);

    // Apply inverse quantum Fourier transform
    for (size_t index = 0; index < n / 2; index++) {
        circuit.addSwapGate(index, n - index - 1);
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < i; j++) {
            circuit.addControlledPhaseGate(j, i, std::numbers::pi / pow(2, i - j));
        }
        circuit.addHadamardGate(i);
    }
    return circuit;
}

/// @brief Shor's Algorithm for N = 15
///
/// This example showcases the usage of the library to implement Shor's Algorithm
/// Shor's Algorithm is a quantum algorithm that aims to find the period of a function defined as:
/// f(x) = a^x mod N
/// where a and N are integers and a is coprime to N
/// The period of the function is the smallest positive integer r such that f(x) = f(x + r) = 1
Circuit<double>::CompoundResult shorsAlgorithm(unsigned long a, unsigned long countingQubits, unsigned long repetitions = 1000) {

    // Create a Quantum Circuit with N counting qubits plus 4 qubits for U to act on.
    const std::shared_ptr<ProbabilityEngine<double>> &probabilityEngine = std::make_shared<ProbabilityEngine<double>>();

    auto circuit = Circuit<double>(probabilityEngine, countingQubits + 4, countingQubits);

    // Initialize counting qubits to |+>
    for (size_t i = 0; i < countingQubits; i++) {
        circuit.addHadamardGate(i);
    }

    // Initialize auxiliary qubit to |1>
    circuit.addXGate(countingQubits);

    // Apply controlled-U operations
    for (size_t i = 0; i < countingQubits; i++) {
        // Make a controlled U gate
        auto uGate = UGate(a, (unsigned long)pow(2, i));
        uGate.setQubitIndices({countingQubits, countingQubits + 1, countingQubits + 2, countingQubits + 3});
        auto cuGate = uGate.makeControlled(i);
        circuit.addGate(std::move(cuGate));
    }

    // Apply inverse quantum Fourier transform
    std::vector<size_t> qftQubits;
    qftQubits.reserve(countingQubits);
    for (size_t i = 0; i < countingQubits; i++) {
        qftQubits.push_back(i);
    }
    circuit += CQFT(probabilityEngine, countingQubits);

    // Measure counting qubits
    for (size_t i = 0; i < countingQubits; i++) {
        circuit.addMeasureGate({{i, i}});
    }

    std::cout << circuit << std::endl;
    return circuit.simulate(repetitions);
}