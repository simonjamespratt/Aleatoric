#include "DiscreteGenerator.hpp"
#include "../Engine/Engine.hpp" //TODO: check if the include paths can be added to the compilation

actlib::Numbers::DiscreteGenerator::DiscreteGenerator(std::vector<double> distribution) : discreteDistribution(distribution.begin(), distribution.end()) {}

actlib::Numbers::DiscreteGenerator::~DiscreteGenerator() {}

int actlib::Numbers::DiscreteGenerator::getNumber() {
    return discreteDistribution(actlib::RNG::engine);
}

void actlib::Numbers::DiscreteGenerator::setDistribution(std::vector<double> distribution) {
    discreteDistribution = std::discrete_distribution<int>(distribution.begin(), distribution.end());
}
