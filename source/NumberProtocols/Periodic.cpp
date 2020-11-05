#include "Periodic.hpp"

#include "ErrorChecker.hpp"

#include <assert.h>
#include <math.h>  // round
#include <numeric> // std::accumulate

namespace aleatoric {
Periodic::Periodic(std::unique_ptr<IDiscreteGenerator> generator)
: m_generator(std::move(generator)),
  m_range(0, 1),
  m_periodicity(0.0),
  m_haveRequestedFirstNumber(false)
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

Periodic::Periodic(std::unique_ptr<IDiscreteGenerator> generator,
                   Range range,
                   double chanceOfRepetition)
: m_generator(std::move(generator)),
  m_range(range),
  m_periodicity(chanceOfRepetition),
  m_haveRequestedFirstNumber(false)
{
    if(chanceOfRepetition < 0.0 || chanceOfRepetition > 1.0) {
        throw std::invalid_argument(
            "The value passed as argument for chanceOfRepetition must be "
            "within the range of 0.0 - 1.0");
    }

    m_generator->setDistributionVector(m_range.size, 1.0);
}

Periodic::~Periodic()
{}

int Periodic::getIntegerNumber()
{
    auto generatedNumber = m_generator->getNumber();
    setPeriodicDistribution(generatedNumber);
    m_lastReturnedNumber = generatedNumber + m_range.offset;
    m_haveRequestedFirstNumber = true;
    return m_lastReturnedNumber;
}

double Periodic::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Periodic::reset()
{
    m_generator->updateDistributionVector(1.0);
    m_haveRequestedFirstNumber = false;
}

NumberProtocolParameters Periodic::getParams()
{
    return NumberProtocolParameters(
        m_range,
        NumberProtocolParameters::Protocols(
            NumberProtocolParameters::Periodic(m_periodicity)));
}

void Periodic::setParams(NumberProtocolParameters params)
{
    auto chanceOfRepetition =
        params.protocols.getPeriodic().getChanceOfRepetition();

    if(chanceOfRepetition < 0.0 || chanceOfRepetition > 1.0) {
        throw std::invalid_argument(
            "The value passed as argument for chanceOfRepetition must be "
            "within the range of 0.0 - 1.0");
    }

    // NB: order is important here. Periodicity must be set before range in
    // order for recalculation of distibution to be done using the new
    // periodicity
    m_periodicity = chanceOfRepetition;
    setRange(params.getRange());
}

// Private methods
double Periodic::calculateRemainerAllocation()
{
    auto vectorSize = m_generator->getDistributionVector().size();
    return (1.0 - m_periodicity) / (vectorSize - 1.0);
}

void Periodic::setPeriodicDistribution(int selectedIndex)
{
    // The total of all values in the vector must equal 1.0.
    // The value at the index of the last selected number
    // must have the value of the periodicity (chanceOfRepetition).
    // The remainder of 1.0 - periodicity is shared equally amongst
    // the remaining vector indices.

    auto distributionVector = m_generator->getDistributionVector();
    auto remainderAllocation = calculateRemainerAllocation();

    for(int i = 0; i < distributionVector.size(); i++) {
        auto newVectorValue =
            i == selectedIndex ? m_periodicity : remainderAllocation;
        distributionVector[i] = newVectorValue;
    }

    auto vectorValuesTotal = std::accumulate(distributionVector.begin(),
                                             distributionVector.end(),
                                             0.0);

    // TODO: DOUBLE-SUMMING-PRECISION: Have needed to use round here because the
    // resulting value of vectorValuesTotal from std::accumulate SPORADICALLY
    // (!??) evaluates to not exactly 1.0. At time of writing I don't know why
    // that is!
    assert(round(vectorValuesTotal) == 1.0);

    m_generator->setDistributionVector(distributionVector);
}

void Periodic::setRange(Range newRange)
{
    m_range = newRange;

    m_generator->setDistributionVector(m_range.size, 1.0);

    if(m_haveRequestedFirstNumber &&
       m_range.numberIsInRange(m_lastReturnedNumber)) {
        setPeriodicDistribution(m_lastReturnedNumber - m_range.offset);
    }
}

} // namespace aleatoric
