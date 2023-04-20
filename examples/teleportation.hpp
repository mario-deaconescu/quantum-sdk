#include <memory>

#include "../include/circuit.hpp"

inline void runQuantumTeleportationExample(){
    // Create a Quantum Circuit with 3 qubits and 2 classical bits
    const std::shared_ptr<QPP::ProbabilityEngine<double>> &probabilityEngine = std::make_shared<QPP::ProbabilityEngine<double>>();
    auto circuit = QPP::Circuit<double>(probabilityEngine, 3, 2);

    std::cout << "Quantum Teleportation Example" << std::endl << std::endl;

    std::cout << "First step: Initialize the qubit we want to teleport to a random state" << std::endl << std::endl;

    // Initialize the first qubit in a random state
    circuit.addInitGate(0, QPP::Qubit<double>::State::Random(probabilityEngine));

    std::cout << circuit << std::endl << std::endl;

    std::cout << "Second step: Create an entangled pair of qubits" << std::endl << std::endl;

    circuit.addHadamardGate(1);
    circuit.addCXGate(1, 2);

    std::cout << circuit << std::endl << std::endl;

    std::cout << "Third step: Entangle the teleportation qubit to the second qubit" << std::endl << std::endl;

    circuit.addCXGate(0, 1);
    circuit.addHadamardGate(0);

    std::cout << circuit << std::endl << std::endl;

    std::cout << "Fourth step: Measure the first two qubits" << std::endl << std::endl;

    // Measure the first two qubits
    circuit.addMeasureGate({{0, 0}, {1, 1}});

    std::cout << circuit << std::endl << std::endl;

    std::cout << "Fifth step: Apply the appropriate gates based on the measurement results" << std::endl << std::endl;

    // Apply the appropriate gates based on the measurement results
    circuit.addGate(std::make_unique<QPP::Circuit<double>::XGate>(2)->makeControlled(1, true));
    circuit.addGate(std::make_unique<QPP::Circuit<double>::ZGate>(2)->makeControlled(0, true));

    std::cout << circuit << std::endl << std::endl;
}