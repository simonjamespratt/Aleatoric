#include "UniformGenerator.hpp"

namespace actlib { namespace Numbers {
    UniformGenerator::UniformGenerator(std::mt19937& engine, int rangeStart, int rangeEnd) :
        _engine(engine),
        _distribution(rangeStart, rangeEnd)
    {}

    UniformGenerator::~UniformGenerator() {}

    int UniformGenerator::getNumber() {
        return _distribution(_engine);
    }

    void UniformGenerator::setDistribution(int startRange, int endRange) {
        _distribution = std::uniform_int_distribution<int>(startRange, endRange);
    }
}}
