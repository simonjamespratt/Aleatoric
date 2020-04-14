#include "Precision.hpp"

#include "ErrorChecker.hpp"

#include <numeric> // std::accumulate

namespace actlib { namespace Numbers { namespace Steps {
Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     std::unique_ptr<Range> range,
                     std::vector<double> distribution)
: m_generator(std::move(generator)), m_range(std::move(range))
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

    if(distribution.size() != m_range->size) {
        throw std::invalid_argument("The vector size for the distribution must "
                                    "match the size of the provided range");
    }

    m_generator->setDistributionVector(distribution);
    m_haveInitialSelection = false;
    m_haveRequestedFirstNumber = false;
}

Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     std::unique_ptr<Range> range,
                     std::vector<double> distribution,
                     int initialSelection)
: Precision(std::move(generator), std::move(range), distribution)
{
    actlib::ErrorChecker::checkInitialSelectionInRange(initialSelection,
                                                       *m_range);

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Precision::~Precision()
{}

int Precision::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        m_haveRequestedFirstNumber = true;
        return m_initialSelection;
    }

    return m_generator->getNumber() + m_range->offset;
}

void Precision::reset()
{
    if(m_haveInitialSelection) {
        m_haveRequestedFirstNumber = false;
    }
}
}}} // namespace actlib::Numbers::Steps
