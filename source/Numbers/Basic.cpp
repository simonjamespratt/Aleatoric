#include "Basic.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Basic::Basic(std::unique_ptr<IUniformGenerator> generator,
             std::unique_ptr<Range> range)
: m_range(std::move(range)), m_generator(std::move(generator))
{
    m_generator->setDistribution(m_range->start, m_range->end);
}

Basic::~Basic()
{}

int Basic::getNumber()
{
    return m_generator->getNumber();
}

void Basic::reset()
{
    // NB: Basic's implementation of the Protocol interface doesn't require
    // reset to do anything.
}
}}} // namespace actlib::Numbers::Steps
