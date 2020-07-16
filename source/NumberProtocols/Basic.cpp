#include "Basic.hpp"

namespace aleatoric {
Basic::Basic(std::unique_ptr<IUniformGenerator> generator, Range range)
: m_range(range), m_generator(std::move(generator))
{
    m_generator->setDistribution(m_range.start, m_range.end);
}

Basic::~Basic()
{}

int Basic::getIntegerNumber()
{
    return m_generator->getNumber();
}

double Basic::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Basic::reset()
{
    // NB: Basic's implementation of the Protocol interface doesn't require
    // reset to do anything.
}

void Basic::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistribution(m_range.start, m_range.end);
}

Range Basic::getRange()
{
    return m_range;
}
} // namespace aleatoric
