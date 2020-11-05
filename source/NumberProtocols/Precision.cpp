#include "Precision.hpp"

#include "ErrorChecker.hpp"

#include <numeric> // std::accumulate

namespace aleatoric {
Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator)
: m_generator(std::move(generator)), m_range(0, 1)
{
    m_generator->setDistributionVector(std::vector<double> {0.5, 0.5});
}

Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     Range range,
                     std::vector<double> distribution)
: m_generator(std::move(generator)), m_range(range)
{
    checkDistributionIsValid(distribution);
    checkDistributionMatchesRange(distribution, m_range);
    m_generator->setDistributionVector(distribution);
}

Precision::~Precision()
{}

int Precision::getIntegerNumber()
{
    return m_generator->getNumber() + m_range.offset;
}

double Precision::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Precision::reset()
{}

void Precision::setParams(NumberProtocolParameters newParams)
{
    auto newDistribution = newParams.protocols.getPrecision().getDistribution();
    auto newRange = newParams.getRange();
    checkDistributionIsValid(newDistribution);
    checkDistributionMatchesRange(newDistribution, newRange);
    m_generator->setDistributionVector(newDistribution);
    m_range = newRange;
}

NumberProtocolParameters Precision::getParams()
{
    return NumberProtocolParameters(
        m_range,
        NumberProtocolParameters::Protocols(NumberProtocolParameters::Precision(
            m_generator->getDistributionVector())));
}

// Private methods
void Precision::checkDistributionIsValid(
    const std::vector<double> &distribution)
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
}

void Precision::checkDistributionMatchesRange(
    const std::vector<double> &distribution, const Range &range)
{
    if(distribution.size() != range.size) {
        throw std::invalid_argument("The vector size for the distribution must "
                                    "match the size of the provided range");
    }
}
} // namespace aleatoric
