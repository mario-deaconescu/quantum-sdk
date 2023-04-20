#include <iostream>

#include "../include/qubit.hpp"
#include "../include/circuit.hpp"

#include "shors_algorithm.hpp"


int main(){

    /// This examples showcases the basic usage of the library

    // Instantiate the probability engine with the desired floating point type
    std::shared_ptr<QPP::ProbabilityEngine<double>> probabilityEngine = std::make_shared<QPP::ProbabilityEngine<double>>();

    // Create a single qubit and inject the probability engine into it
    auto qubit = QPP::Qubit<double>(probabilityEngine);
    std::cout << "Qubit: \n" << qubit << "\n\n";

    auto classicBit = QPP::ClassicBit();
    classicBit.setState(QPP::ClassicBit::ONE);
    std::cout << "Classic bit: \n" << classicBit << "\n\n";

    // Create a circuit and inject the probability engine into it
    // This circuit will have 2 qubits and 2 classical bits
    auto circuit = QPP::Circuit<double>(probabilityEngine, 2, 2);

    // Add a Hadamard gate to the first qubit, which will enter the superposition state
    circuit.addHadamardGate(0);

    // Add a CNOT gate to the first and second qubits, which will entangle them
    circuit.addCXGate(0, 1);


    // Add a measurement gate to the first and second qubits, which will measure them by collapsing them to a classical bit
    circuit.addMeasureGate({{0, 0}, {1, 1}});

    // Print the circuit
    std::cout << "Circuit: \n" << circuit << "\n";

    // Simulate the circuit 1000 times
    std::cout << "Simulation results: \n" << circuit.simulate(10000) << "\n";


    // Simulate Shor's algorithm for N = 7, a = 4, and 10000 iterations
    std::cout << shorsAlgorithm(7, 4, 10000) << std::endl;

    return 0;
}