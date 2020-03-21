#include "Serial.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Serial::Serial(std::unique_ptr<IDiscreteGenerator> generator,
               std::unique_ptr<Range> range)
: m_range(std::move(range)), m_generator(std::move(generator))
{
    m_generator->setDistributionVector(m_range->size, 1.0);
}

Serial::~Serial()
{}

int Serial::getNumber()
{
    if(seriesIsComplete()) {
        reset();
    }

    int selectedNumber = m_generator->getNumber();
    m_generator->updateDistributionVector(selectedNumber, 0.0);
    return selectedNumber + m_range->offset;
}

void Serial::reset()
{
    // Set all vector values back to uniform (equal probability)
    m_generator->updateDistributionVector(1.0);
}

bool Serial::seriesIsComplete()
{
    auto distributionVector = m_generator->getDistributionVector();
    for(auto &&item : distributionVector) {
        if(item > 0.0) {
            return false;
        }
    }
    return true;
}
}}} // namespace actlib::Numbers::Steps
