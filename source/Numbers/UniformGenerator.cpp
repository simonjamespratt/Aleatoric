#include "UniformGenerator.hpp"
#include "../Engine/Engine.hpp" //TODO: check if the include paths can be added to the compilation

actlib::Numbers::UniformGenerator::UniformGenerator(int rangeStart, int rangeEnd) : uniformDistribution(rangeStart, rangeEnd) {}

actlib::Numbers::UniformGenerator::~UniformGenerator() {}

int actlib::Numbers::UniformGenerator::getNumber() {
    return uniformDistribution(actlib::RNG::engine);
}

void actlib::Numbers::UniformGenerator::setDistribution(int startRange, int endRange) {
    uniformDistribution = std::uniform_int_distribution<int>(startRange, endRange);
}
