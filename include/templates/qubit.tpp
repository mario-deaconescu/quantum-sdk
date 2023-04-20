template<std_floating_point FloatingNumberType>
void Qubit<FloatingNumberType>::State::assertValid() const {
    const std::complex<FloatingNumberType> total = norm(alpha) + norm(beta);
    if (!probabilityEngine->template compare<std::complex<FloatingNumberType>>(total, 1.0)) {
        throw Qubit<FloatingNumberType>::State::InvalidStateException(*this);
    }
}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine)
        : alpha(1.0), beta(0.0), probabilityEngine(probabilityEngine) {
    assertValid();
}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine,
                                        const std::complex<FloatingNumberType> &alpha,
                                        const std::complex<FloatingNumberType> &beta): alpha(alpha), beta(beta),
                                                                                       probabilityEngine(
                                                                                               probabilityEngine) {
    assertValid();
}

template<std_floating_point FloatingNumberType>
void Qubit<FloatingNumberType>::State::set(const std::complex<FloatingNumberType> &alpha_,
                                           const std::complex<FloatingNumberType> &beta_) {
    this->alpha = alpha_;
    this->beta = beta_;
    assertValid();
}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::State::State(const State &other): alpha(other.alpha), beta(other.beta),
                                                             probabilityEngine(other.probabilityEngine) {}

template<std_floating_point FloatingNumberType>
typename Qubit<FloatingNumberType>::State &
Qubit<FloatingNumberType>::State::operator=(const Qubit<FloatingNumberType>::State &other) {
    this->alpha = other.alpha;
    this->beta = other.beta;
    return *this;
}

template<std_floating_point FloatingNumberType>
const std::complex<FloatingNumberType> &Qubit<FloatingNumberType>::State::getAlpha() const {
    return alpha;
}

template<std_floating_point FloatingNumberType>
const std::complex<FloatingNumberType> &Qubit<FloatingNumberType>::State::getBeta() const {
    return beta;
}

template<std_floating_point FloatingNumberType>
std::string Qubit<FloatingNumberType>::State::getRepresentation() const {
    return "(" + std::to_string(alpha.real()) + " + " + std::to_string(alpha.imag()) + "i)" + "×❘0〉"
           + "+ "
           + "(" + std::to_string(beta.real()) + " + " + std::to_string(beta.imag()) + "i)" + "×❘1〉";
}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::Qubit(const std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine): state(
        probabilityEngine),
                                                                                                                  probabilityEngine(
                                                                                                                          probabilityEngine) {}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::Qubit(const Qubit &qubit) = default;

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType> &Qubit<FloatingNumberType>::operator=(const Qubit &qubit) {
    this->state = qubit.state;
    return *this;
}


template<std_floating_point FloatingNumberType>
void Qubit<FloatingNumberType>::setState(const State &state_) {
    this->state = state_;
}

template<std_floating_point FloatingNumberType>
void Qubit<FloatingNumberType>::setState(const std::complex<FloatingNumberType> &alpha,
                                         const std::complex<FloatingNumberType> &beta) {
    this->state = State(probabilityEngine, alpha, beta);
}

template<std_floating_point FloatingNumberType>
const typename Qubit<FloatingNumberType>::State &Qubit<FloatingNumberType>::getState() const {
    return state;
}

template<std_floating_point FloatingNumberType>
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

template<std_floating_point FloatingNumberType>
std::string Qubit<FloatingNumberType>::getRepresentation() const {
    return state.getRepresentation();
}

template<std_floating_point FloatingNumberType>
Qubit<FloatingNumberType>::State::InvalidStateException::InvalidStateException(const State &state) :
        std::runtime_error(
                "Invalid state vector: " +
                state.getRepresentation() +
                "\n" +
                "|α|^2 + |β|^2 = " +
                std::to_string(
                        norm(state.getAlpha()) +
                        norm(state.getBeta()))),
        state(state) {}

template<std_floating_point FloatingNumberType>
typename Qubit<FloatingNumberType>::State Qubit<FloatingNumberType>::State::Random(
        std::shared_ptr<ProbabilityEngine<FloatingNumberType>> probabilityEngine) {
    const double p0 = probabilityEngine->getProbability();
    const double p1 = 1.0 - p0;

    const double phi0 = 2.0 * std::numbers::pi * probabilityEngine->getProbability();
    const double phi1 = 2.0 * std::numbers::pi * probabilityEngine->getProbability();

    const auto alpha = std::sqrt(p0) * std::complex<double>(std::cos(phi0), std::sin(phi0));
    const auto beta = std::sqrt(p1) * std::complex<double>(std::cos(phi1), std::sin(phi1));

    return State(probabilityEngine, alpha, beta);
}
