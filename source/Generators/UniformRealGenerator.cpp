#include "UniformRealGenerator.hpp"

#include "Engine.hpp"

namespace aleatoric {
UniformRealGenerator::UniformRealGenerator()
: m_engine(std::make_unique<Engine>()),
  m_distribution(0.0, 1.0),
  m_range(0.0, 1.0)
{}

UniformRealGenerator::UniformRealGenerator(double rangeStart, double rangeEnd)
: m_engine(std::make_unique<Engine>()),
  m_distribution(rangeStart, rangeEnd),
  m_range(rangeStart, rangeEnd)
{}

UniformRealGenerator::~UniformRealGenerator()
{}

double UniformRealGenerator::getNumber()
{
    return m_distribution(m_engine->getEngine());
}

void UniformRealGenerator::setDistribution(double rangeStart, double rangeEnd)
{
    m_range = std::make_pair(rangeStart, rangeEnd);
    m_distribution =
        std::uniform_real_distribution<double>(rangeStart, rangeEnd);
}

std::pair<double, double> UniformRealGenerator::getDistribution()
{
    return m_range;
}
} // namespace aleatoric
