#include "Walk.hpp"

#include "ErrorChecker.hpp"
#include "Utilities.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers { namespace Steps {

Walk::Walk(std::unique_ptr<IUniformGenerator> generator,
           std::unique_ptr<Range> range,
           int maxStep)
: m_range(std::move(range)),
  m_generator(std::move(generator)),
  m_maxStep(maxStep),
  m_haveInitialSelection(false),
  m_haveRequestedFirstNumber(false)
{
    if(maxStep > m_range->size) {
        throw std::invalid_argument("The value passed as argument for maxStep "
                                    "must be less than or equal to " +
                                    std::to_string(m_range->size));
    }

    m_generator->setDistribution(m_range->start, m_range->end);
}

Walk::Walk(std::unique_ptr<IUniformGenerator> generator,
           std::unique_ptr<Range> range,
           int maxStep,
           int initialSelection)
: Walk(std::move(generator), std::move(range), maxStep)
{
    actlib::ErrorChecker::checkInitialSelectionInRange(initialSelection,
                                                       *m_range);

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

    auto generatedNumber = m_generator->getNumber();
    setForNextStep(generatedNumber);
    return generatedNumber;
}

void Walk::reset()
{
    m_generator->setDistribution(m_range->start, m_range->end);
    m_haveRequestedFirstNumber = false;
}

void Walk::setForNextStep(int lastSelectedNumber)
{
    auto newSubRange = actlib::Utilities::getMaxStepSubRange(lastSelectedNumber,
                                                             m_maxStep,
                                                             m_range->start,
                                                             m_range->end);

    m_generator->setDistribution(std::get<0>(newSubRange),
                                 std::get<1>(newSubRange));
}

}}} // namespace actlib::Numbers::Steps
