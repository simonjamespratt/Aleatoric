#include "GranularWalk.hpp"

#include "ErrorChecker.hpp"
#include "Utilities.hpp"

#include <math.h>

namespace aleatoric {
GranularWalk::GranularWalk(std::unique_ptr<UniformRealGenerator> generator)
: m_generator(std::move(generator)),
  m_range(0, 1),
  m_deviationFactor(1.0),
  m_haveRequestedFirstNumber(false)
{
    initialise();
}

GranularWalk::GranularWalk(std::unique_ptr<UniformRealGenerator> generator,
                           Range range,
                           double deviationFactor)
: m_generator(std::move(generator)),
  m_range(range),
  m_deviationFactor(deviationFactor),
  m_haveRequestedFirstNumber(false)
{
    ErrorChecker::checkValueWithinUnitInterval(m_deviationFactor,
                                               "deviationFactor");

    initialise();
}

GranularWalk::~GranularWalk()
{}

int GranularWalk::getIntegerNumber()
{
    auto decimalNumber = getDecimalNumber();
    auto roundedNumber = round(decimalNumber);
    return static_cast<int>(roundedNumber);
}

double GranularWalk::getDecimalNumber()
{
    m_lastReturnedNumber = m_generator->getNumber();
    setForNextStep();
    m_haveRequestedFirstNumber = true;

    return m_lastReturnedNumber;
}

void GranularWalk::setParams(NumberProtocolParameters newParams)
{
    auto granWalkParams = newParams.protocols.getGranularWalk();

    ErrorChecker::checkValueWithinUnitInterval(
        granWalkParams.getDeviationFactor(),
        "deviationFactor");

    m_deviationFactor = granWalkParams.getDeviationFactor();
    setMaxStep();
    setRange(newParams.getRange());
}

NumberProtocolParameters GranularWalk::getParams()
{
    return NumberProtocolParameters(
        m_range,
        NumberProtocolParameters::Protocols(
            NumberProtocolParameters::GranularWalk(m_deviationFactor)));
}

// Private methods=====================================================
void GranularWalk::setForNextStep()
{
    auto newRangeStart = m_lastReturnedNumber - m_maxStep;

    if(!m_range.floatingPointIsInRange(newRangeStart)) {
        newRangeStart = static_cast<double>(m_range.start);
    }

    auto newRangeEnd = m_lastReturnedNumber + m_maxStep;

    if(!m_range.floatingPointIsInRange(newRangeEnd)) {
        newRangeEnd = static_cast<double>(m_range.end);
    }

    m_generator->setDistribution(newRangeStart, newRangeEnd);
}

void GranularWalk::setMaxStep()
{
    auto rangeEnd = static_cast<double>(m_range.end);
    auto rangeStart = static_cast<double>(m_range.start);

    m_maxStep = (rangeEnd - rangeStart) * m_deviationFactor;
}

void GranularWalk::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistribution(m_range.start, m_range.end);

    if(m_range.floatingPointIsInRange(m_lastReturnedNumber)) {
        setForNextStep();
    }
}

void GranularWalk::initialise()
{
    setMaxStep();
    m_generator->setDistribution(m_range.start, m_range.end);
}

} // namespace aleatoric
