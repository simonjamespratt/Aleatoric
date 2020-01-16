#include "Walk.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers { namespace Steps {

Walk::Walk(IUniformGenerator &generator, Range &range, int maxStep)
: m_range(range),
  m_generator(generator),
  m_maxStep(maxStep),
  m_haveInitialSelection(false),
  m_haveRequestedFirstNumber(false)
{
    if(maxStep > range.size) {
        throw std::invalid_argument("The value passed as argument for maxStep "
                                    "must be less than or equal to " +
                                    std::to_string(range.size));
    }
}

Walk::Walk(IUniformGenerator &generator,
           Range &range,
           int maxStep,
           int initialSelection)
: Walk(generator, range, maxStep)
{
    // TODO: this initialSelection check turns up a lot now. Should really
    // derive a custom exception from invalid_argument specifically for this and
    // use it wherever this check is made.
    if(initialSelection < range.start || initialSelection > range.end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(range.start) + " to " + std::to_string(range.end));
    }

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Walk::~Walk()
{}

int Walk::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        setForNextStep(m_initialSelection);
        m_haveRequestedFirstNumber = true;
        return m_initialSelection;
    }

    auto generatedNumber = m_generator.getNumber();
    setForNextStep(generatedNumber);
    return generatedNumber;
}

void Walk::reset()
{
    m_generator.setDistribution(m_range.start, m_range.end);
    m_haveRequestedFirstNumber = false;
}

void Walk::setForNextStep(int lastSelectedNumber)
{
    auto newRangeStart = lastSelectedNumber - m_maxStep;
    if(isOutOfRange(newRangeStart)) {
        newRangeStart = m_range.start;
    }

    auto newRangeEnd = lastSelectedNumber + m_maxStep;
    if(isOutOfRange(newRangeEnd)) {
        newRangeEnd = m_range.end;
    }

    m_generator.setDistribution(newRangeStart, newRangeEnd);
}

bool Walk::isOutOfRange(int number)
{
    return number < m_range.start || number > m_range.end;
};

}}} // namespace actlib::Numbers
