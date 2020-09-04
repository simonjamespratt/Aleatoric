#include "NoRepetition.hpp"

namespace aleatoric {
NoRepetition::NoRepetition(std::unique_ptr<IDiscreteGenerator> generator,
                           Range range)
: m_generator(std::move(generator)),
  m_range(range),
  m_haveRequestedFirstNumber(false)
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

NoRepetition::~NoRepetition()
{}

int NoRepetition::getIntegerNumber()
{
    auto generatedNumber = m_generator->getNumber();
    // reset equal probability
    m_generator->updateDistributionVector(1.0);
    // disallow last selected number
    m_generator->updateDistributionVector(generatedNumber, 0.0);
    m_lastNumberReturned = generatedNumber + m_range.offset;
    m_haveRequestedFirstNumber = true;
    return m_lastNumberReturned;
}

double NoRepetition::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void NoRepetition::reset()
{
    m_generator->updateDistributionVector(1.0);
}

void NoRepetition::setRange(Range newRange)
{
    auto oldRange = m_range;
    auto lastNumberGenerated = m_lastNumberReturned - oldRange.offset;

    m_generator->setDistributionVector(newRange.size, 1.0);

    if(m_haveRequestedFirstNumber &&
       newRange.numberIsInRange(m_lastNumberReturned)) {
        m_generator->updateDistributionVector(lastNumberGenerated, 0.0);
    }

    m_range = newRange;
}

Range NoRepetition::getRange()
{
    return m_range;
}

} // namespace aleatoric
