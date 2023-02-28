template <typename FloatingNumberType>
void Qubit<FloatingNumberType>::State::assertValid() const {
    const std::complex<FloatingNumberType> total = alpha * alpha + beta * beta;
    if (!probabilityEngine->template compare<std::complex<FloatingNumberType>>(total, 1.0)){
        throw Qubit<FloatingNumberType>::State::InvalidStateException(*this);
    }
}

template <typename FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine): alpha(1.0), beta(0.0), probabilityEngine(probabilityEngine) {
    assertValid();
}

template <typename FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine, const std::complex<FloatingNumberType>& alpha, const std::complex<FloatingNumberType>& beta): alpha(alpha), beta(beta), probabilityEngine(probabilityEngine){
    assertValid();
}

template <typename FloatingNumberType>
void Qubit<FloatingNumberType>::State::set(const std::complex<FloatingNumberType>& alpha_, const std::complex<FloatingNumberType>& beta_){
    this->alpha = alpha_;
    this->beta = beta_;
    assertValid();
}

template <typename FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const State& other): alpha(other.alpha), beta(other.beta), probabilityEngine(other.probabilityEngine) {}

template <typename FloatingNumberType>
typename Qubit<FloatingNumberType>::State& Qubit<FloatingNumberType>::State::operator=(const Qubit<FloatingNumberType>::State& other){
    this->alpha = other.alpha;
    this->beta = other.beta;
    return *this;
}

template <typename FloatingNumberType>
const std::complex<FloatingNumberType>& Qubit<FloatingNumberType>::State::getAlpha() const {
    return alpha;
}

template <typename FloatingNumberType>
const std::complex<FloatingNumberType>& Qubit<FloatingNumberType>::State::getBeta() const {
    return beta;
}

template <typename FloatingNumberType>
std::string Qubit<FloatingNumberType>::State::getRepresentation() const {
    return "(" + std::to_string(alpha.real()) + " + " + std::to_string(alpha.imag()) + "i)" + "×❘0〉"
           + "+ "
           + "(" + std::to_string(beta.real()) + " + " + std::to_string(beta.imag()) + "i)" + "×❘1〉";
}

template <typename FloatingNumberType>
Qubit<FloatingNumberType>::Qubit(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine): state(probabilityEngine),
                                                                                                                   probabilityEngine(probabilityEngine) {}

template <typename FloatingNumberType>
Qubit<FloatingNumberType>::Qubit(const Qubit& qubit) = default;

template <typename FloatingNumberType>
Qubit<FloatingNumberType>& Qubit<FloatingNumberType>::operator=(const Qubit& qubit){
    this->state = qubit.state;
    return *this;
}


template <typename FloatingNumberType>
void Qubit<FloatingNumberType>::setState(const State& state_) {
    this->state = state_;
}

template <typename FloatingNumberType>
void Qubit<FloatingNumberType>::setState(const std::complex<FloatingNumberType>& alpha, const std::complex<FloatingNumberType>& beta) {
    this->state = State(probabilityEngine, alpha, beta);
}

template <typename FloatingNumberType>
const typename Qubit<FloatingNumberType>::State& Qubit<FloatingNumberType>::getState() const {
    return state;
}

template <typename FloatingNumberType>
ClassicBit Qubit<FloatingNumberType>::measure() {
    const double zeroProbability = std::norm(state.getAlpha());
    const double randomizedValue = probabilityEngine->getProbability();
    if (randomizedValue < zeroProbability) {
        state.set(1.0, 0.0);
        return ClassicBit(ClassicBit::State::ZERO);
    } else {
        state.set(0.0, 1.0);
        return ClassicBit(ClassicBit::State::ONE);
    }
}

template<typename FloatingNumberType>
std::string Qubit<FloatingNumberType>::getRepresentation() const {
    return state.getRepresentation();
}

template<typename FloatingNumberType>
Qubit<FloatingNumberType>::State::InvalidStateException::InvalidStateException(const State &state) : state(state) {
    message = "Invalid state vector: " + state.getRepresentation();
}

template<typename FloatingNumberType>
const char *Qubit<FloatingNumberType>::State::InvalidStateException::what() const noexcept {
    return message.c_str();
}