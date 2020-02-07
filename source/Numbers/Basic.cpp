#include "Basic.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Basic::Basic(IUniformGenerator &generator, Range &range)
: m_range(range), m_generator(generator)
{
    m_generator.setDistribution(m_range.start, m_range.end);
}

Basic::~Basic()
{}

int Basic::getNumber()
{
    return m_generator.getNumber();
}

void Basic::reset()
{
    // TODO: Basic's implementation of the Protocol interface doesn't require
    // reset to do anything. So for now, this is an empty method definition.
    // Consider whether this is the correct solution.
}
}}} // namespace actlib::Numbers::Steps
