<h1 align="center">
    Q++ - A Quantum Computing SDK
</h1>

<p align="center">
  <img src="https://mariodeaconescu.com/assets/q++.svg" alt="angular-logo" width="120px" height="120px"/>
  <br>
<i>
Q++ is a C++ library designed to simulate Quantum Computer behaviour by implementing concepts such as quantum circuits, qubits and more.
</i>
  <br>
</p>
<p align="center">
  <strong>This library was inspired by <a href="https://qiskit.org/">Qiskit</a>.</strong>
</p>

# Documentation

Currently, the only available documentation is the [Doxygen](https://www.doxygen.nl/index.html) generated one as well as the one found here.

Additional information can be found in the ```examples/``` folder, which contains a few examples of how to use the library.

# Basic Functionality

## Qubits

- Qubits are the basic building blocks of quantum computing. 
  They can be in a superposition of states, meaning that they can be in a state of 0 and 1 at the same time.
- Their state can be represented by two complex numbers, α and β, which are called amplitudes.
- These amplitudes determine the probability of the qubit collapsing to 0 or 1 respectively, and as such, the sum of their squares must be equal to 1.
- Qubits can be measured, which will collapse the superposition and return a 0 or 1.
- Qubits can be entangled, which means that they can be in a superposition of states with other qubits.
- The ```Qubit``` class is used to represent a qubit and its functionality.

## Classic Bits

- Classic bits are the regular bits that we are used to, which can only be in a state of 0 or 1.
- The ```ClassicBit``` class is used to represent a classic bit and its functionality.
- The ```qubit.measure()``` method is used to measure a qubit and return a ```ClassicBit```.

## Quantum Gates

- Quantum gates are used to manipulate qubits.
- They can be used to apply a transformation to a qubit, or to entangle two qubits.
- The ```Gate``` class is used to represent a quantum gate and its functionality.
- The ```Gate``` class is an abstract class, and as such, it cannot be instantiated, but it can be inherited from.

## Quantum Circuits

- Quantum circuits are used to represent a sequence of quantum gates that are applied to a set of qubits.
- The ```Circuit``` class is used to represent a quantum circuitPointer and its functionality.
- Circuits can be run once, or multiple times, and the results can be used to gather statistics.

## Probability Engine

- As seen above, probability is a key concept in quantum computing. For this reason, we need a powerful object that can efficiently calculate probabilities.
- The ```ProbabilityEngine``` is what we use to achieve this.
- It works by dependency injection, meaning that it must be provided to any class that needs to calculate probabilities, such as ```Circuit``` or ```Qubit```.
- A big issue with probability is that it is not always possible to calculate it exactly, due to the fact that the numbers involved are very large (or rather have many decimals).
  To combat this, the ```ProbabilityEngine``` has an ```errorMargin``` that can be provided to it, which will be used to determine whether two values are equal or not.
- Another solution that has been implemented is the use of a custom number type for every class. (Only tested for ```float```, ```double``` and ```long double```).

## Visualisation

- All classes have a ```getRepresentation()``` method, which returns a string representation of the object.
- This can be used to visualise the state of the qubits, the circuitPointer, etc.
- Additionally, every class can be printed to the console using the ```<<``` operator.
