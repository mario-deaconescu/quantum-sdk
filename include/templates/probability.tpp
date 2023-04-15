template<std_floating_point FloatingNumberType>
ProbabilityEngine<FloatingNumberType>::ProbabilityEngine(): device(std::random_device()), distribution(0.0, 1.0) {}

template<std_floating_point FloatingNumberType>
FloatingNumberType ProbabilityEngine<FloatingNumberType>::getProbability() {
    std::mt19937_64 generator(device());
    return distribution(generator);
}

template<std_floating_point FloatingNumberType>
ProbabilityEngine<FloatingNumberType>::ProbabilityEngine(const FloatingNumberType& errorMargin_): errorMargin(errorMargin_) {}