#include "Serial.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Serial::Serial(IDiscreteGenerator &generator, Range &range)
: _range(range), _generator(generator)
{}

Serial::~Serial()
{}

int Serial::getNumber()
{
    if(seriesIsComplete()) {
        reset();
    }

    int selectedNumber = _generator.getNumber();
    _generator.updateDistributionVector(selectedNumber, 0.0);
    return selectedNumber + _range.offset;
}

void Serial::reset()
{
    // Set all vector values back to uniform (equal probability)
    _generator.updateDistributionVector(1.0);
}

bool Serial::seriesIsComplete()
{
    auto distributionVector = _generator.getDistributionVector();
    for(auto &&item : distributionVector) {
        if(item > 0.0) {
            return false;
        }
    }
    return true;
}
}}} // namespace actlib::Numbers::Steps
