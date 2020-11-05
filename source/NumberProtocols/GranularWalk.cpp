#include "GranularWalk.hpp"

#include "ErrorChecker.hpp"
#include "Utilities.hpp"

#include <math.h>

namespace aleatoric {
GranularWalk::GranularWalk(std::unique_ptr<IUniformGenerator> generator)
: m_generator(std::move(generator)),
  m_externalRange(0, 1),
  m_internalRange(0, 65000),
  m_deviationFactor(1.0),
  m_haveRequestedFirstNumber(false)
{
    initialise();
}

GranularWalk::GranularWalk(std::unique_ptr<IUniformGenerator> generator,
                           Range range,
                           double deviationFactor)
: m_generator(std::move(generator)),
  m_externalRange(range),
  m_internalRange(0, 65000),
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
    auto selectedNumber = m_generator->getNumber();
    setForNextStep(selectedNumber);
    m_haveRequestedFirstNumber = true;

    return m_lastReturnedNumber =
               mapToNewRange(selectedNumber, m_internalRange, m_externalRange);
}

void GranularWalk::reset()
{
    m_generator->setDistribution(m_internalRange.start, m_internalRange.end);
    m_haveRequestedFirstNumber = false;
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
        m_externalRange,
        NumberProtocolParameters::Protocols(
            NumberProtocolParameters::GranularWalk(m_deviationFactor)));
}

// Private methods=====================================================
double
GranularWalk::scaleToRange(double normalizedValue, int rangeMin, int rangeMax)
{
    return (normalizedValue * (rangeMax - rangeMin)) + rangeMin;
}

double GranularWalk::normalize(int value, int rangeMin, int rangeMax)
{
    return static_cast<double>(value - rangeMin) /
           static_cast<double>(rangeMax - rangeMin);
}

double GranularWalk::mapToNewRange(double value, Range fromRange, Range toRange)
{
    auto normalized = normalize(value, fromRange.start, fromRange.end);
    return scaleToRange(normalized, toRange.start, toRange.end);
}

void GranularWalk::setForNextStep(int lastSelectedNumber)
{
    auto newSubRange = Utilities::getMaxStepSubRange(lastSelectedNumber,
                                                     m_maxStep,
                                                     m_internalRange.start,
                                                     m_internalRange.end);

    m_generator->setDistribution(std::get<0>(newSubRange),
                                 std::get<1>(newSubRange));
}

void GranularWalk::setMaxStep()
{
    double maxStep = scaleToRange(m_deviationFactor,
                                  m_internalRange.start,
                                  m_internalRange.end);
    m_maxStep = static_cast<int>(round(maxStep));
}

void GranularWalk::setRange(Range newRange)
{
    m_externalRange = newRange;

    if(!m_haveRequestedFirstNumber) {
        return;
    }

    if(m_externalRange.numberIsInRange(m_lastReturnedNumber)) {
        auto mappedValue = mapToNewRange(m_lastReturnedNumber,
                                         m_externalRange,
                                         m_internalRange);

        setForNextStep(static_cast<int>(round(mappedValue)));

    } else {
        m_generator->setDistribution(m_internalRange.start,
                                     m_internalRange.end);
    }
}

void GranularWalk::initialise()
{
    setMaxStep();
    m_generator->setDistribution(m_internalRange.start, m_internalRange.end);
}

} // namespace aleatoric
