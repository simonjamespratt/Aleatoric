#include "UniformRealGenerator.hpp"

namespace aleatoric {
UniformRealGenerator::UniformRealGenerator()
: m_distribution(0.0, 1.0), m_engine(m_seedSource), m_range(0.0, 1.0)
{}

UniformRealGenerator::UniformRealGenerator(double rangeStart, double rangeEnd)
: m_distribution(rangeStart, rangeEnd),
  m_engine(m_seedSource),
  m_range(rangeStart, rangeEnd)
{}

UniformRealGenerator::~UniformRealGenerator()
{}

double UniformRealGenerator::getNumber()
{
    return m_distribution(m_engine);
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
