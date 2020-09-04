#include "AdjacentSteps.hpp"

#include "ErrorChecker.hpp"

namespace aleatoric {
AdjacentSteps::AdjacentSteps(std::unique_ptr<IDiscreteGenerator> generator,
                             Range range)
: m_generator(std::move(generator)),
  m_range(range),
  m_haveRequestedFirstNumber(false)
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

AdjacentSteps::~AdjacentSteps()
{}

int AdjacentSteps::getIntegerNumber()
{
    auto generatedNumber = m_generator->getNumber();
    m_lastReturnedNumber = generatedNumber + m_range.offset;

    m_haveRequestedFirstNumber = true;

    // step configuring in preparation for next call
    prepareStepBasedDistribution(m_lastReturnedNumber);

    return m_lastReturnedNumber;
}

double AdjacentSteps::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void AdjacentSteps::reset()
{
    m_generator->updateDistributionVector(1.0);
    m_haveRequestedFirstNumber = false;
}

void AdjacentSteps::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistributionVector(m_range.size, 1.0);

    if(m_haveRequestedFirstNumber &&
       newRange.numberIsInRange(m_lastReturnedNumber)) {
        prepareStepBasedDistribution(m_lastReturnedNumber);
    }
}

Range AdjacentSteps::getRange()
{
    return m_range;
}

void AdjacentSteps::prepareStepBasedDistribution(int number)
{
    auto vectorIndex = number - m_range.offset;
    m_generator->updateDistributionVector(0.0);

    if(number == m_range.start) {
        m_generator->updateDistributionVector(vectorIndex + 1, 1.0);

    } else if(number == m_range.end) {
        m_generator->updateDistributionVector(vectorIndex - 1, 1.0);

    } else {
        m_generator->updateDistributionVector(vectorIndex + 1, 1.0);
        m_generator->updateDistributionVector(vectorIndex - 1, 1.0);
    }
}
} // namespace aleatoric
