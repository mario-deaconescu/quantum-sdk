template <typename FloatingNumberType>
ProbabilityEngine<FloatingNumberType>::ProbabilityEngine(): device(std::random_device()), distribution(0.0, 1.0) {}

template <typename FloatingNumberType>
FloatingNumberType ProbabilityEngine<FloatingNumberType>::getProbability() {
    std::mt19937_64 generator(device());
    return distribution(generator);
}

template <typename FloatingNumberType>
ProbabilityEngine<FloatingNumberType>::ProbabilityEngine(const FloatingNumberType& errorMargin_): errorMargin(errorMargin_) {}