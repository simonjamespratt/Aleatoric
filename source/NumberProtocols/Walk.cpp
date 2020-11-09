#include "Walk.hpp"

#include "ErrorChecker.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace aleatoric {
Walk::Walk(std::unique_ptr<IUniformGenerator> generator)
: m_generator(std::move(generator)),
  m_range(0, 1),
  m_maxStep(1),
  m_haveRequestedFirstNumber(false)
{
    m_generator->setDistribution(m_range.start, m_range.end);
}

Walk::Walk(std::unique_ptr<IUniformGenerator> generator,
           Range range,
           int maxStep)
: m_range(range),
  m_generator(std::move(generator)),
  m_maxStep(maxStep),
  m_haveRequestedFirstNumber(false)
{
    checkMaxStepIsValid(maxStep, m_range);

    m_generator->setDistribution(m_range.start, m_range.end);
}

Walk::~Walk()
{}

int Walk::getIntegerNumber()
{
    auto generatedNumber = m_generator->getNumber();
    setForNextStep(generatedNumber);
    m_lastNumberSelected = generatedNumber;

    m_haveRequestedFirstNumber = true;

    return m_lastNumberSelected;
}

double Walk::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Walk::reset()
{
    m_generator->setDistribution(m_range.start, m_range.end);
    m_haveRequestedFirstNumber = false;
}

void Walk::setParams(NumberProtocolParameters newParams)
{
    auto maxStep = newParams.protocols.getWalk().getMaxStep();
    auto newRange = newParams.getRange();

    checkMaxStepIsValid(maxStep, newRange);

    m_maxStep = maxStep;
    setRange(newRange);
}

NumberProtocolParameters Walk::getParams()
{
    return NumberProtocolParameters(
        m_range,
        NumberProtocolParameters::Protocols(
            NumberProtocolParameters::Walk(m_maxStep)));
}

// Private methods
void Walk::setForNextStep(int lastSelectedNumber)
{
    auto newRangeStart = lastSelectedNumber - m_maxStep;
    if(!m_range.numberIsInRange(newRangeStart)) {
        newRangeStart = m_range.start;
    }

    auto newRangeEnd = lastSelectedNumber + m_maxStep;
    if(!m_range.numberIsInRange(newRangeEnd)) {
        newRangeEnd = m_range.end;
    }

    m_generator->setDistribution(newRangeStart, newRangeEnd);
}

void Walk::checkMaxStepIsValid(int maxStep, Range range)
{
    if(maxStep < 1 || maxStep > range.size) {
        throw std::invalid_argument("The value passed as argument for maxStep "
                                    "must be less than or equal to " +
                                    std::to_string(range.size));
    }
}

void Walk::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistribution(m_range.start, m_range.end);

    if(m_haveRequestedFirstNumber &&
       m_range.numberIsInRange(m_lastNumberSelected)) {
        setForNextStep(m_lastNumberSelected);
    }
}

} // namespace aleatoric
