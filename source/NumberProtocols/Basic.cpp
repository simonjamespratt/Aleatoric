#include "Basic.hpp"

namespace aleatoric {
Basic::Basic(std::unique_ptr<IUniformGenerator> generator,
             std::unique_ptr<Range> range)
: m_range(std::move(range)), m_generator(std::move(generator))
{
    m_generator->setDistribution(m_range->start, m_range->end);
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
} // namespace aleatoric
