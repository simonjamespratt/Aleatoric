#include "Precision.hpp"

#include <stdexcept>

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

void Precision::setParams(NumberProtocolParameters newParams)
{
    auto newDistribution = newParams.protocols.getPrecision().getDistribution();
    auto newRange = newParams.getRange();
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
void Precision::checkDistributionMatchesRange(
    const std::vector<double> &distribution, const Range &range)
{
    if(static_cast<int>(distribution.size()) != range.size) {
        throw std::invalid_argument("The vector size for the distribution must "
                                    "match the size of the provided range");
    }
}
} // namespace aleatoric
