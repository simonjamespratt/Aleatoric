#include "UniformGenerator.hpp"

namespace aleatoric {
UniformGenerator::UniformGenerator()
: m_distribution(0, 1), m_engine(m_seedSource)
{}

UniformGenerator::UniformGenerator(int rangeStart, int rangeEnd)
: m_distribution(rangeStart, rangeEnd), m_engine(m_seedSource)
{}

UniformGenerator::~UniformGenerator()
{}

int UniformGenerator::getNumber()
{
    return m_distribution(m_engine);
}

void UniformGenerator::setDistribution(int startRange, int endRange)
{
    m_distribution = std::uniform_int_distribution<int>(startRange, endRange);
}
} // namespace aleatoric
