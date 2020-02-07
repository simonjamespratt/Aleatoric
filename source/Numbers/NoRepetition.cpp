#include "NoRepetition.hpp"

namespace actlib { namespace Numbers { namespace Steps {
NoRepetition::NoRepetition(IDiscreteGenerator &generator, Range &range)
: m_generator(generator), m_range(range)
{
    m_generator.setDistributionVector(m_range.size, 1.0);
}

NoRepetition::~NoRepetition()
{}

int NoRepetition::getNumber()
{
    auto generatedNumber = m_generator.getNumber();
    // reset equal probability
    m_generator.updateDistributionVector(1.0);
    // disallow last selected number
    m_generator.updateDistributionVector(generatedNumber, 0.0);
    return generatedNumber + m_range.offset;
}

void NoRepetition::reset()
{
    m_generator.updateDistributionVector(1.0);
}

}}} // namespace actlib::Numbers::Steps
