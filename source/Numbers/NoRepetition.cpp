#include "NoRepetition.hpp"

namespace actlib { namespace Numbers { namespace Steps {
NoRepetition::NoRepetition(std::unique_ptr<IDiscreteGenerator> generator,
                           std::unique_ptr<Range> range)
: m_generator(std::move(generator)), m_range(std::move(range))
{
    m_generator->setDistributionVector(m_range->size, 1.0);
}

NoRepetition::~NoRepetition()
{}

int NoRepetition::getNumber()
{
    auto generatedNumber = m_generator->getNumber();
    // reset equal probability
    m_generator->updateDistributionVector(1.0);
    // disallow last selected number
    m_generator->updateDistributionVector(generatedNumber, 0.0);
    return generatedNumber + m_range->offset;
}

void NoRepetition::reset()
{
    m_generator->updateDistributionVector(1.0);
}

}}} // namespace actlib::Numbers::Steps
