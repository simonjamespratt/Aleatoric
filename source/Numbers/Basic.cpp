#include "Basic.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Basic::Basic(IUniformGenerator &generator, Range &range)
: _range(range), _generator(generator)
{}

Basic::~Basic()
{}

int Basic::getNumber()
{
    return _generator.getNumber();
}

void Basic::reset()
{
    // TODO: Basic's implementation of the Protocol interface doesn't require
    // reset to do anything. So for now, this is an empty method definition.
    // Consider whether this is the correct solution.
}
}}} // namespace actlib::Numbers::Steps
