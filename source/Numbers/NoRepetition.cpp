#include "NoRepetition.hpp"

namespace actlib { namespace Numbers { namespace Steps {
NoRepetition::NoRepetition(IDiscreteGenerator &generator, Range &range)
: _generator(generator), _range(range)
{}

NoRepetition::~NoRepetition()
{}

int NoRepetition::getNumber()
{
    auto generatedNumber = _generator.getNumber();
    // reset equal probability
    _generator.updateDistributionVector(1.0);
    // disallow last selected number
    _generator.updateDistributionVector(generatedNumber, 0.0);
    return generatedNumber + _range.offset;
}

void NoRepetition::reset()
{
    _generator.updateDistributionVector(1.0);
}

}}} // namespace actlib::Numbers
