#include "UniformGenerator.hpp"

#include "Engine.hpp"

namespace aleatoric {
UniformGenerator::UniformGenerator()
: m_engine(std::make_unique<Engine>()), m_distribution(0, 1)
{}

UniformGenerator::UniformGenerator(int rangeStart, int rangeEnd)
: m_engine(std::make_unique<Engine>()), m_distribution(rangeStart, rangeEnd)
{}

UniformGenerator::~UniformGenerator()
{}

int UniformGenerator::getNumber()
{
    return m_distribution(m_engine->getEngine());
}

void UniformGenerator::setDistribution(int startRange, int endRange)
{
    m_distribution = std::uniform_int_distribution<int>(startRange, endRange);
}
} // namespace aleatoric
