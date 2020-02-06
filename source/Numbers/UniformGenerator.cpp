#include "UniformGenerator.hpp"

namespace actlib { namespace Numbers {
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
}} // namespace actlib::Numbers
