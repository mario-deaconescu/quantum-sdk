/// @file circuitPointer.h
/// @brief This file contains the Circuit class template, the abstract class Gate, and all of the available Gates.
///
/// A Circuit is a sequence of gates that can be applied to a set of qubits.
/// The Circuit class template is a container for the gates and the qubits.
/// It also contains a ProbabilityEngine that is used to generate random numbers.
/// The Circuit class template is used to represent a quantum circuitPointer.
/// It can be used to simulate a quantum circuitPointer and to draw it
///
/// @author Mario Deaconescu

#pragma once

#include<string>
#include<vector>
#include<array>
#include<map>
#include<memory>
#include<iostream>
#include "qubit.hpp"

/// @class Gate
/// @brief An abstract class representing a gate.
/// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
template<typename FloatingNumberType>
class Gate;

/// @class Circuit
/// @brief A class template representing a quantum circuitPointer.
/// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
template<typename FloatingNumberType>
class Circuit : public Representable{
private:

    class Gate : public Representable{
        friend class Circuit<FloatingNumberType>;
    public :
        typedef std::vector<std::array<std::string, 3>> Drawings;
        /// @brief Applies the gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the gate to.
        virtual void apply(Circuit<FloatingNumberType> *circuit) = 0;
        /// @brief Clone the gate.
        /// @return A pointer to the cloned gate.
        [[nodiscard]]
        virtual std::unique_ptr<Gate> clone() const = 0;
        /// @brief Check if the gate is valid with respect to the given circuit.
        /// @param circuit The circuit to check the gate against.
        /// @return True if the gate is valid, false otherwise.
        virtual void assertValid(const Circuit* circuit) const = 0;
        /// @brief Get a controlled version of the gate.
        /// @param controlIndex The index of the control qubit.
        /// @return A pointer to the controlled gate.
        [[nodiscard]] std::unique_ptr<Gate> makeControlled(const size_t& controlIndex) const;
    protected:
        /// @brief Returns a standard string representation of the gate based on an identifier.
        /// @param identifier The identifier of the gate.
        /// @return A standard string representation of the gate.
        [[nodiscard]] static Drawings getStandardDrawing(const Circuit<FloatingNumberType> *circuit, const std::string& identifier, const size_t& qubitIndex);
        /// @brief Returns a string representation of the gate.
        /// @return A string representation of the gate.
        [[nodiscard]] virtual Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const = 0;
    };

    class ControlledGate : public Gate{
    protected:
        const size_t controlIndex;
        [[nodiscard]] typename Gate::Drawings getStandardDrawing(const Circuit<FloatingNumberType> *circuit, const std::string& identifier, const size_t& qubitIndex) const;
    public:
        explicit ControlledGate(const size_t& controlIndex);
        [[nodiscard]] bool getControlState(Circuit<FloatingNumberType> *circuit) const;
        void assertValid(const Circuit* circuit) const override;
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


    template <typename DerivedGate>
    [[deprecated]]
    std::unique_ptr<Gate> makeClone(const DerivedGate& derived) {
        static_assert(std::is_base_of<Gate, DerivedGate>::value, "DerivedGate must be a subclass of Gate");
        return std::make_unique<DerivedGate>(derived);
    }

    std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine;

    std::vector<Qubit<FloatingNumberType>> qubits;
    std::vector<ClassicBit> classicBits;
    std::vector<std::unique_ptr<Gate>> gates;
public:

    /// @brief Holds the result of a circuitPointer run.
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

    /// @brief Holds the result of multiple circuitPointer runs.
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
        /// @param circuit The circuitPointer to draw the gate in.
        /// @return A vector of drawings of the measure gate.
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a MeasureGate with the given vector of qubit-classic bit pairs.
        /// @param qubitClassicBitPairs The vector of qubit-classic bit pairs.
        explicit MeasureGate(const std::vector<std::pair<size_t, size_t>>& qubitClassicBitPairs);

        /// @brief Returns a string representation of the measure gate.
        /// @return A string representation of the measure gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the measure gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the measure gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class HadamardGate
    /// @brief A class representing a Hadamard gate.
    ///
    /// A Hadamard gate is a gate that applies a Hadamard transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class HadamardGate : public Gate {
    private:
    protected:
        size_t qubitIndex;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a HadamardGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit HadamardGate(const size_t& qubitIndex);

        /// @brief Returns a string representation of the Hadamard gate.
        /// @return A string representation of the Hadamard gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Hadamard gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Hadamard gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class ControlledHadamardGate
    /// @brief A class representing a controlled Hadamard gate.
    ///
    /// A controlled Hadamard gate is a gate that applies a Hadamard transformation to a qubit if the control qubit is in the |1> state.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class ControlledHadamardGate : public ControlledGate, public HadamardGate {
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a CXGate with the given control qubit index and target qubit index.
        /// @param controlQubitIndex The control qubit index.
        /// @param targetQubitIndex The target qubit index.
        ControlledHadamardGate(const size_t& controlIndex, const size_t& targetIndex);
        /// @brief Returns a string representation of the CNOT gate.
        /// @return A string representation of the CNOT gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CNOT gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the CNOT gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class XGate
    /// @brief A class representing a NOT gate.
    ///
    /// A NOT gate is a gate that applies a NOT transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class XGate : public Gate {
    protected:
        size_t qubitIndex;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a XGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit XGate(const size_t& qubitIndex);

        /// @brief Returns a string representation of the NOT gate.
        /// @return A string representation of the NOT gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the NOT gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the NOT gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class CXGate
    /// @brief A class representing a CNOT gate.
    ///
    /// A CNOT gate is a gate that applies a NOT transformation to a qubit if the state of a control qubit is 1.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CXGate : public ControlledGate, public XGate {
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a CXGate with the given control qubit index and target qubit index.
        /// @param controlQubitIndex The control qubit index.
        /// @param targetQubitIndex The target qubit index.
        CXGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);
        /// @brief Returns a string representation of the CNOT gate.
        /// @return A string representation of the CNOT gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CNOT gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the CNOT gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class YGate
    /// @brief A class representing a Y gate.
    ///
    /// A Y gate is a gate that applies a Y transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class YGate : public Gate {
    protected:
        size_t qubitIndex;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a YGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit YGate(const size_t& qubitIndex);

        /// @brief Returns a string representation of the Y gate.
        /// @return A string representation of the Y gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Y gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Y gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class CYGate
    /// @brief A class representing a CY gate.
    ///
    /// A CY gate is a gate that applies a Y transformation to a qubit if the state of a control qubit is 1.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CYGate : public ControlledGate, public YGate {
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:

        /// @brief Creates a CYGate with the given control qubit index and target qubit index.
        /// @param controlQubitIndex The control qubit index.
        /// @param targetQubitIndex The target qubit index.
        CYGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);
        /// @brief Returns a string representation of the CY gate.
        /// @return A string representation of the CY gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CY gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the CY gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class ZGate
    /// @brief A class representing a Z gate.
    ///
    /// A Z gate is a gate that applies a Z transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class ZGate : public Gate {
    protected:
        size_t qubitIndex;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a ZGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit ZGate(const size_t& qubitIndex);

        /// @brief Returns a string representation of the Z gate.
        /// @return A string representation of the Z gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Z gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Z gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class CZGate
    /// @brief A class representing a CZ gate.
    ///
    /// A CZ gate is a gate that applies a Z transformation to a qubit if the state of a control qubit is 1.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CZGate : public ControlledGate, public ZGate {
    protected:
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a CZGate with the given control qubit index and target qubit index.
        /// @param controlQubitIndex The control qubit index.
        /// @param targetQubitIndex The target qubit index.
        CZGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);
        /// @brief Returns a string representation of the CZ gate.
        /// @return A string representation of the CZ gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CZ gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the CZ gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class SwapGate
    /// @brief A class representing a Swap gate.
    ///
    /// A Swap gate is a gate that swaps the state of two qubits.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class SwapGate : public Gate {
    protected:
        size_t qubitIndex1;
        size_t qubitIndex2;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
        class SwapSameQubitException : public std::exception {
        protected:
            size_t qubitIndex;
            std::string message;
        public:
            explicit SwapSameQubitException(const size_t& qubitIndex);
            [[nodiscard]] const char* what() const noexcept override;
        };
    public:
        /// @brief Creates a SwapGate with the given qubit indices.
        /// @param qubitIndex1 The first qubit index.
        /// @param qubitIndex2 The second qubit index.
        SwapGate(const size_t& qubitIndex1, const size_t& qubitIndex2);
        /// @brief Returns a string representation of the Swap gate.
        /// @return A string representation of the Swap gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Swap gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Swap gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class CustomControlledGate
    /// @brief A class representing a CustomControlledGate.
    ///
    /// A CustomControlledGate is a gate that applies a custom gate to a qubit if the state of a control qubit is 1.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CustomControlledGate : public Gate {
    protected:
        size_t controlIndex;
        std::unique_ptr<Gate> gatePointer;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a CustomControlledGate with the given control qubit index and gate.
        /// @param controlQubitIndex The control qubit index.
        /// @param gate The gate to apply to the target qubit if the control qubit is 1.
        CustomControlledGate(const size_t& controlQubitIndex, std::unique_ptr<Gate> gate);

        /// @brief Copy constructor.
        /// @param other The CustomControlledGate to copy.
        CustomControlledGate(const CustomControlledGate& other);


        /// @brief Returns a string representation of the CustomControlledGate.
        /// @return A string representation of the CustomControlledGate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the CustomControlledGate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the CustomControlledGate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class CircuitGate
    /// @brief A class representing a CircuitGate.
    ///
    /// A CircuitGate is a gate that runs a circuit on a set of qubits.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class CircuitGate : public Gate {
    private:
        static size_t activeInstances;
    protected:
        std::shared_ptr<Circuit<FloatingNumberType>> circuitPointer;
        std::vector<size_t> qubitIndices;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
        class InvalidQubitIndicesException : public std::exception {
        public:
            explicit InvalidQubitIndicesException(const CircuitGate& gate, const std::vector<size_t>& indices);
            [[nodiscard]] const char* what() const noexcept override;

        private:
            const size_t qubitCount;
            const size_t indexCount;
            const std::string message;
        };
    public:

        /// @brief The name of the CircuitGate.
        /// @details The name of the CircuitGate is used to identify the gate in the circuitPointer.
        /// Has default value "NEW_CIRCUIT_GATE".
        std::string name = "NEW_CIRCUIT_GATE";

        static void issueUninitializedCircuitGateWarning();
        /// @brief Creates a CircuitGate with the given circuit pointer.
        /// @param circuit The circuitPointer pointer.
        explicit CircuitGate(std::shared_ptr<Circuit<FloatingNumberType>> circuit);

        /// @brief Creates a CircuitGate with the given circuit and a name.
        /// @param circuit The circuitPointer.
        /// @param name The name of the CircuitGate.
        CircuitGate(const Circuit<FloatingNumberType>& circuit, const std::string& name);

        /// @brief Creates a CircuitGate with the given circuitPointer.
        /// @param circuit The circuitPointer.
        explicit CircuitGate(const Circuit<FloatingNumberType>& circuit);
        /// @brief Creates a CircuitGate with the given circuitPointer pointer and qubit indices.
        /// @param circuit The circuitPointer pointer.
        /// @param indices The qubit indices.
        CircuitGate(std::shared_ptr<Circuit<FloatingNumberType>> circuit, const std::vector<size_t>& indices);

        /// @brief Sets the qubit indices corresponding to the qubits in the circuit.
        /// @param indices The qubit indices.
        void setQubitIndices(const std::vector<size_t>& indices);

        /// @brief Gets the number of qubits in the circuit.
        /// @return The number of qubits in the circuit.
        [[nodiscard]] size_t getQubitCount() const;

        /// @brief Returns a string representation of the Circuit gate.
        /// @return A string representation of the Circuit gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Circuit gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Circuit gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class PhaseGate
    /// @brief A class representing a Phase gate.
    ///
    /// A Phase gate is a gate that applies a phase transformation to a qubit.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class PhaseGate : public Gate {
    protected:
        size_t qubitIndex;
        double angle;
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a PhaseGate with the given qubit index.
        /// @param qubitIndex The qubit index.
        explicit PhaseGate(const size_t& qubitIndex, const double& angle);
        /// @brief Returns a string representation of the Phase gate.
        /// @return A string representation of the Phase gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Phase gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Phase gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @class ControlledPhaseGate
    /// @brief A class representing a Controlled Phase gate.
    ///
    /// A Controlled Phase gate is a gate that applies a phase transformation to a qubit if the control qubit is in the |1> state.
    /// @tparam FloatingNumberType The type of the floating-point number used to represent the probabilities.
    class ControlledPhaseGate : public ControlledGate, public PhaseGate {
    protected:
        size_t controlQubitIndex{};
        [[nodiscard]] typename Gate::Drawings getDrawings(const Circuit<FloatingNumberType> *circuit) const override;
    public:
        /// @brief Creates a ControlledPhaseGate with the given control qubit index.
        /// @param controlQubitIndex The control qubit index.
        explicit ControlledPhaseGate(const size_t& controlQubitIndex, const size_t& qubitIndex, const double& angle);
        /// @brief Returns a string representation of the Controlled Phase gate.
        /// @return A string representation of the Controlled Phase gate.
        [[nodiscard]] std::string getRepresentation() const override;

        /// @brief Applies the Controlled Phase gate to the given circuitPointer.
        /// @param circuit The circuitPointer to apply the Controlled Phase gate to.
        void apply(Circuit<FloatingNumberType> *circuit) override;

        void assertValid(const Circuit* circuit) const override;

        std::unique_ptr<Gate> clone() const override;
    };

    /// @brief Creates a Circuit with the given probability engine, qubit count and classic bit count.
    /// @param probabilityEngine The probability engine to use.
    /// @param qubitCount The qubit count.
    /// @param classicBitCount The classic bit count.
    Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine, const size_t& qubitCount, const size_t& classicBitCount);

    /// @brief Creates a Circuit with the given probability engine and qubit count.
    /// @param probabilityEngine The probability engine to use.
    /// @param qubitCount The qubit count.
    Circuit(std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine, const size_t& qubitCount);

    Circuit(const Circuit& other);

    /// @brief Returns a drawing representation of the circuit.
    /// @return A drawing representation of the circuit.
    [[nodiscard]] std::string getRepresentation() const override;

    /// @brief Resets the circuit.
    ///
    /// @details Resets the circuit by resetting the qubits.
    void reset();

    /// @brief Adds an already constructed CircuitGate to the circuit.
    /// @param gate The CircuitGate to add.
    void addGate(std::unique_ptr<CircuitGate> gate, const std::vector<size_t>& qubitIndices);

    /// @brief Adds a gate to the circuit.
    /// @param gate The pointer to the gate to add.
    void addGate(std::unique_ptr<Gate> gate);

    /// @brief Adds a measure gate to the circuit.
    /// @param qubitClassicBitPairs The vector of qubit-classic bit pairs.
    void addMeasureGate(const std::vector<std::pair<size_t, size_t>>& qubitClassicBitPairs);

    /// @brief Adds a Hadamard gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addHadamardGate(const size_t& qubitIndex);

    /// @brief Adds a controlled Hadamard gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addControlledHadamardGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);

    /// @brief Adds a NOT gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addXGate(const size_t& qubitIndex);


    /// @brief Adds a CNOT gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addCXGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);

    /// @brief Adds a Y gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addYGate(const size_t& qubitIndex);

    /// @brief Adds a CY gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addCYGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);

    /// @brief Adds a Z gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addZGate(const size_t& qubitIndex);

    /// @brief Adds a CZ gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addCZGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex);

    /// @brief Adds a Swap gate to the circuit.
    /// @param qubitIndex1 The first qubit index.
    /// @param qubitIndex2 The second qubit index.
    void addSwapGate(const size_t& qubitIndex1, const size_t& qubitIndex2);

    /// @brief Adds a CircuitGate to the circuit.
    /// @param circuitGate The CircuitGate to add.
    void addCircuitGate(const Circuit<FloatingNumberType>& circuit, const std::vector<size_t>& qubitIndices);

    /// @brief Adds a Phase gate to the circuit.
    /// @param qubitIndex The qubit index.
    void addPhaseGate(const size_t& qubitIndex, const FloatingNumberType& angle);

    /// @brief Adds a Controlled Phase gate to the circuit.
    /// @param controlQubitIndex The control qubit index.
    /// @param targetQubitIndex The target qubit index.
    void addControlledPhaseGate(const size_t& controlQubitIndex, const size_t& targetQubitIndex, const FloatingNumberType& angle);

    void operator+=(const Circuit& other);

    CircuitGate toGate() const;

    /// @brief Runs the circuitPointer.
    /// @return The result of the circuitPointer.
    Result run();

    /// @brief Simulates the circuitPointer a number of times.
    /// @param count The number of times to simulate the circuitPointer.
    /// @return The compound result of the simulation.
    CompoundResult simulate(const size_t& count);
};




#include "templates/circuit.tpp"
#include "templates/measure.tpp"
#include "templates/xgate.tpp"
#include "templates/ygate.tpp"
#include "templates/zgate.tpp"
#include "templates/hadamard.tpp"
#include "templates/swap.tpp"
#include "templates/circuit_gate.tpp"
#include "templates/phase.tpp"
#include "templates/control.tpp"











