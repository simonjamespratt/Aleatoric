#include "Precision.hpp"

#include "ErrorChecker.hpp"

#include <numeric> // std::accumulate

namespace aleatoric {
Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     Range range,
                     std::vector<double> distribution)
: m_generator(std::move(generator)), m_range(range)
{
    double sumDistValues =
        std::accumulate(distribution.begin(), distribution.end(), 0.0);

    // TODO: DOUBLE-SUMMING-PRECISION: The following check is flawed. Due to
    // issues with precision, certain numbers will not sum exactly to 1.0. See
    // https://stackoverflow.com/questions/588004/is-floating-point-math-broken
    // for info. An example here would be a vector of size 10 where each entry
    // is 0.1. This does not sum to 1.0. It will require tolerance checks etc.
    // as described in the SO article linked to. I suspect this is also the root
    // of the problem with a similar issue in Periodic.cpp

    // The unit tests for this should be altered to test the end fix for this.
    // Also the Numbers factory method tests and the Integrations tests for
    // Precision should be reinstated to use a range of 0,9 as is used for all
    // other protocols under test.
    if(sumDistValues != 1.0) {
        throw std::invalid_argument(
            "The sum of the values provided as the vector for the "
            "distribution must equal 1.0");
    }

    if(distribution.size() != m_range.size) {
        throw std::invalid_argument("The vector size for the distribution must "
                                    "match the size of the provided range");
    }

    m_generator->setDistributionVector(distribution);
    m_haveInitialSelection = false;
    m_haveRequestedFirstNumber = false;
}

Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     Range range,
                     std::vector<double> distribution,
                     int initialSelection)
: Precision(std::move(generator), range, distribution)
{
    ErrorChecker::checkInitialSelectionInRange(initialSelection, m_range);

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Precision::~Precision()
{}

int Precision::getIntegerNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        m_haveRequestedFirstNumber = true;
        return m_initialSelection;
    }

    return m_generator->getNumber() + m_range.offset;
}

double Precision::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Precision::reset()
{
    if(m_haveInitialSelection) {
        m_haveRequestedFirstNumber = false;
    }
}

void Precision::setRange(Range newRange)
{
    auto oldRange = m_range;
    m_range = newRange;

    // TODO: DYNAMIC-PARAMS: this is an interim measure and should be
    // updated when the solution for altering all params at the same
    // time is implemented.
    auto distribution = m_generator->getDistributionVector();

    if(newRange.size > distribution.size()) {
        auto difference = newRange.size - distribution.size();
        for(int i = 0; i < difference; i++) {
            distribution.push_back(0.0);
        }
        m_generator->setDistributionVector(distribution);
    }

    // NB: as it stands, this produces a set of dist values that doesn't add up
    // to 1. Not putting it right because it's an interim measure. But if, for
    // whatever reason, it ends up staying, then it'll need some maths to work
    // out how the chopped off values should be redistributed.
    if(newRange.size < distribution.size()) {
        auto difference = distribution.size() - newRange.size;
        for(int i = 0; i < difference; i++) {
            distribution.pop_back();
        }
        m_generator->setDistributionVector(distribution);
    }
}

Range Precision::getRange()
{
    return m_range;
}
} // namespace aleatoric
