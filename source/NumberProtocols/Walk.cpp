#include "Walk.hpp"

#include "ErrorChecker.hpp"
#include "Utilities.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace aleatoric {

Walk::Walk(std::unique_ptr<IUniformGenerator> generator,
           Range range,
           int maxStep)
: m_range(range),
  m_generator(std::move(generator)),
  m_maxStep(maxStep),
  m_haveInitialSelection(false),
  m_haveRequestedFirstNumber(false)
{
    if(maxStep > m_range.size) {
        throw std::invalid_argument("The value passed as argument for maxStep "
                                    "must be less than or equal to " +
                                    std::to_string(m_range.size));
    }

    m_generator->setDistribution(m_range.start, m_range.end);
}

Walk::Walk(std::unique_ptr<IUniformGenerator> generator,
           Range range,
           int maxStep,
           int initialSelection)
: Walk(std::move(generator), range, maxStep)
{
    ErrorChecker::checkInitialSelectionInRange(initialSelection, m_range);

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Walk::~Walk()
{}

int Walk::getIntegerNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        setForNextStep(m_initialSelection);
        m_lastNumberSelected = m_initialSelection;
    } else {
        auto generatedNumber = m_generator->getNumber();
        setForNextStep(generatedNumber);
        m_lastNumberSelected = generatedNumber;
    }

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

void Walk::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistribution(m_range.start, m_range.end);

    if(m_haveRequestedFirstNumber &&
       m_range.numberIsInRange(m_lastNumberSelected)) {
        setForNextStep(m_lastNumberSelected);
    }

    // TODO: DYNAMIC-PARAMS: might want to make sure the maxStep is not >
    // newRange.size when we start updating all params at once. It doesn't break
    // the protocol but it might be something to throw execption about as per in
    // the constructor? Or maybe not?
}

Range Walk::getRange()
{
    return m_range;
}

// Private methods
void Walk::setForNextStep(int lastSelectedNumber)
{
    auto newSubRange = Utilities::getMaxStepSubRange(lastSelectedNumber,
                                                     m_maxStep,
                                                     m_range.start,
                                                     m_range.end);

    m_generator->setDistribution(std::get<0>(newSubRange),
                                 std::get<1>(newSubRange));
}

} // namespace aleatoric
