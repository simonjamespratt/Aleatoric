#include "AdjacentSteps.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

// TODO: Think about whether there is a need for an internal and external range.
// This class brings it into focus. See getNumber() and the addition or
// subtraction of range.offset depending on whether we're talking about the
// number within the context of the range of in the context of the generator's
// vector and the numbers the generator returns. Not sure what the right answer
// is though!

namespace actlib { namespace Numbers { namespace Steps {
AdjacentSteps::AdjacentSteps(std::unique_ptr<IDiscreteGenerator> generator,
                             std::unique_ptr<Range> range)
: m_generator(std::move(generator)), m_range(std::move(range))
{
    m_generator->setDistributionVector(m_range->size, 1.0);
    m_haveInitialSelection = false;
    m_haveRequestedFirstNumber = false;
}

AdjacentSteps::AdjacentSteps(std::unique_ptr<IDiscreteGenerator> generator,
                             std::unique_ptr<Range> range,
                             int initialSelection)
: AdjacentSteps(std::move(generator), std::move(range))
{
    if(initialSelection < m_range->start || initialSelection > m_range->end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(m_range->start) + " to " +
            std::to_string(m_range->end));
    }
    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

AdjacentSteps::~AdjacentSteps()
{}

int AdjacentSteps::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        // step configuring in preparation for next call
        prepareStepBasedDistribution(m_initialSelection,
                                     m_initialSelection - m_range->offset);

        // this only needs to be in here for now as it is only used in the above
        // if statement
        m_haveRequestedFirstNumber = true;

        return m_initialSelection;
    }

    auto generatedNumber = m_generator->getNumber();
    auto numberPlacedWithinRange = generatedNumber + m_range->offset;

    // step configuring in preparation for next call
    prepareStepBasedDistribution(numberPlacedWithinRange, generatedNumber);

    return numberPlacedWithinRange;
}

void AdjacentSteps::reset()
{
    m_generator->updateDistributionVector(1.0);
    m_haveRequestedFirstNumber = false;
}

void AdjacentSteps::prepareStepBasedDistribution(int number, int vectorIndex)
{
    m_generator->updateDistributionVector(0.0);

    if(number == m_range->start) {
        m_generator->updateDistributionVector(vectorIndex + 1, 1.0);

    } else if(number == m_range->end) {
        m_generator->updateDistributionVector(vectorIndex - 1, 1.0);

    } else {
        m_generator->updateDistributionVector(vectorIndex + 1, 1.0);
        m_generator->updateDistributionVector(vectorIndex - 1, 1.0);
    }
}
}}} // namespace actlib::Numbers::Steps
