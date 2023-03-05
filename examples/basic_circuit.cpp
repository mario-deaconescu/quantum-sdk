#include <iostream>

#include "../include/qubit.hpp"
#include "../include/circuit.hpp"

#include "shors_algorithm.hpp"


int main(){

    /// This examples showcases the basic usage of the library

    // Instantiate the probability engine with the desired floating point type
    std::shared_ptr<ProbabilityEngine<double>> probabilityEngine = std::make_shared<ProbabilityEngine<double>>();

    // Create a single qubit and inject the probability engine into it
    auto qubit = Qubit<double>(probabilityEngine);
    std::cout << "Qubit: \n" << qubit << "\n\n";

    auto classicBit = ClassicBit();
    classicBit.setState(ClassicBit::ONE);
    std::cout << "Classic bit: \n" << classicBit << "\n\n";

    // Create a circuit and inject the probability engine into it
    // This circuit will have 2 qubits and 2 classical bits
    auto circuit = Circuit<double>(probabilityEngine, 3, 3);

    // Add a Hadamard gate to the first qubit, which will enter the superposition state

    // Add a CNOT gate to the first and second qubits, which will entangle them
    circuit.addHadamardGate(0);
    circuit.addCXGate(0, 1);
    circuit.addHadamardGate(0);


    // Add a measurement gate to the first and second qubits, which will measure them by collapsing them to a classical bit
    circuit.addMeasureGate({{0, 0}, {1, 1}, {2, 2}});

    // Print the circuit
    std::cout << "Circuit: \n" << circuit << "\n";

    // Simulate the circuit 1000 times
    std::cout << "Simulation results: \n" << circuit.simulate(10000) << "\n";


    // Simulate Sho's algorithm for N = 7, a = 4, and 10000 iterations
    std::cout << shorsAlgorithm(7, 4, 10000) << std::endl;

    return 0;
}