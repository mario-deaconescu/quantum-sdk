/// @file circuit.h
/// @brief This file contains the Circuit class template, the abstract class Gate, and all of the available Gates.
///
/// A Circuit is a sequence of gates that can be applied to a set of qubits.
/// The Circuit class template is a container for the gates and the qubits.
/// It also contains a ProbabilityEngine that is used to generate random numbers.
/// The Circuit class template is used to represent a quantum circuit.
/// It can be used to simulate a quantum circuit and to draw it
///
/// @author Mario Deaconescu

#pragma once

#include<string>
#include<vector>
#include<array>
#include<map>
#include "qubit.hpp"

/// @class Gate
/// @brief An abstract class representing a gate.
/// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
template<typename FloatingNumberType>
class Gate;

/// @class Circuit
/// @brief A class template representing a quantum circuit.
/// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
template<typename FloatingNumberType>
class Circuit : public Representable{
private:

    class Gate : public Representable{
        friend class Circuit<FloatingNumberType>;
    public :
        typedef std::vector<std::array<std::string, 3>> Drawings;
        /// @brief Applies the gate to the given circuit.
        /// @param circuit The circuit to apply the gate to.
        virtual void apply(Circuit<FloatingNumberType> *circuit) = 0;
    protected:
        /// @brief Returns a string representation of the gate.
        /// @return A string representation of the gate.
        [[nodiscard]] virtual Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const = 0;
    };

    class InvalidQubitIndexException : public std::exception{
    public:
        explicit InvalidQubitIndexException(const size_t& qubitIndex);
        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string message;
        const size_t index;
    };

    class InvalidClassicBitIndexException : public std::exception{
    public:
        explicit InvalidClassicBitIndexException(const size_t& classicIndex);
        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string message;
        const size_t index{};
    };

    std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine;

    std::vector<Qubit<FloatingNumberType>> qubits;
    std::vector<ClassicBit> classicBits;
    std::vector<std::unique_ptr<Gate>> gates;
public:

    /// @brief Holds the result of a circuit run.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class Result : public Representable{
    private:
        std::vector<ClassicBit> classicBits;
    public:
        /// @brief Creates a Result with the given vector of classic bits.
        /// @param classicBits The vector of classic bits.
        explicit Result(const std::vector<ClassicBit>& classicBits);
        /// @brief Returns a string representation of the result.
        /// @return A string representation of the result.
        [[nodiscard]] std::string getRepresentation() const override;
    };

    /// @brief Holds the result of multiple circuit runs.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CompoundResult : public Representable{
    private:
        std::map<std::string, size_t> resultMap;
    public:
        /// @brief Creates a CompoundResult with no results.
        CompoundResult();
        /// @brief Creates a CompoundResult with the given vector of results.
        /// @param results The vector of results.
        explicit CompoundResult(const std::vector<Result>& results);
        /// @brief Returns a string representation of the compound result.
        /// @return A string representation of the compound result, similar to a JSON object.
        [[nodiscard]] std::string getRepresentation() const override;
        /// @brief Adds a result to the compound result.
        /// @param result The result to add.
        void addResult(const Result& result);
    };

    /// @class MeasureGate
    /// @brief A class representing a measure gate.
    ///
    /// A measure gate is a gate that measures the state of a qubit (or more) and stores the result in a classic bit (or more).
    ///
    /// The qubit gets collapsed to the state that was measured.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class MeasureGate : public Gate {
    private:
        std::vector<std::pair<size_t, size_t>> qubitClassicBitPairs;
    protected:
        /// @brief Returns a vector of drawings of the measure gate.
        /// @param circuit The circuit to draw the gate in.
        /// @return A vector of drawings of the measure gate.
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a MeasureGate with the given vector of qubit-classic bit pairs.
        /// @param qubitClassicBitPairs The vector of qubit-classic bit pairs.
        explicit MeasureGate(const std::vector<std::pair<size_t, size_t>>& qubitClassicBitPairs);

        /// @brief Returns a string representation of the measure gate.
        /// @return A string representation of the measure gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the measure gate to the given circuit.
        /// @param circuit The circuit to apply the measure gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;
    };

    /// @class HadamardGate
    /// @brief A class representing a Hadamard gate.
    ///
    /// A Hadamard gate is a gate that applies a Hadamard transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class HadamardGate : public Gate {
    private:
        size_t qubitIndex;
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a HadamardGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit HadamardGate(const size_t& qubitIndex);

        /// @brief Returns a string representation of the Hadamard gate.
        /// @return A string representation of the Hadamard gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Hadamard gate to the given circuit.
        /// @param circuit The circuit to apply the Hadamard gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;
    };

    /// @class CNOTGate
    /// @brief A class representing a CNOT gate.
    ///
    /// A CNOT gate is a gate that applies a NOT transformation to a qubit if the state of a control qubit is 1.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CNOTGate : public Gate {
    private:
        size_t controlQubitIndex;
        size_t targetQubitIndex;
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a CNOTGate with the given control qubit index and target qubit index.
        /// @param controlQubitIndex The control qubit index.
        /// @param targetQubitIndex The target qubit index.
        CNOTGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);
        /// @brief Returns a string representation of the CNOT gate.
        /// @return A string representation of the CNOT gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CNOT gate to the given circuit.
        /// @param circuit The circuit to apply the CNOT gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;
    };

    /// @brief Creates a Circuit with the given probability engine, qubit count and classic bit count.
    /// @param probabilityEngine The probability engine to use.
    /// @param qubitCount The qubit count.
    /// @param classicBitCount The classic bit count.
    Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine, const size_t& qubitCount, const size_t& classicBitCount);

    Circuit(const Circuit& circuit) = delete;

    /// @brief Returns a drawing representation of the circuit.
    /// @return A drawing representation of the circuit.
    [[nodiscard]] std::string getRepresentation() const override;

    /// @brief Resets the circuit.
    ///
    /// @details Resets the circuit by resetting the qubits.
    void reset();

    /// @brief Adds a gate to the circuit.
    /// @param gate The gate to add.
    void addGate(std::unique_ptr<Gate> gate);

    /// @brief Adds a measure gate to the circuit.
    /// @param qubitClassicBitPairs The vector of qubit-classic bit pairs.
    void addMeasureGate(const std::vector<std::pair<size_t, size_t>>& qubitClassicBitPairs);

    /// @brief Adds a Hadamard gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addHadamardGate(const size_t& qubitIndex);

    /// @brief Adds a CNOT gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addCNOTGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);

    /// @brief Runs the circuit.
    /// @return The result of the circuit.
    Result run();

    /// @brief Simulates the circuit a number of times.
    /// @param count The number of times to simulate the circuit.
    /// @return The compound result of the simulation.
    CompoundResult simulate(const size_t& count);
};

#include "templates/circuit.tpp"











