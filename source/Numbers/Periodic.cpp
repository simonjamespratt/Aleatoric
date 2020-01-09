#include "Periodic.hpp"

#include <assert.h>
#include <numeric>   // std::accumulate
#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers {
Periodic::Periodic(IDiscreteGenerator &generator,
                   Range &range,
                   double chanceOfRepetition)
: m_range(range),
  m_generator(generator),
  m_periodicity(chanceOfRepetition),
  m_haveInitialSelection(false),
  m_haveRequestedFirstNumber(false)
{
    if(chanceOfRepetition < 0.0 || chanceOfRepetition > 1.0) {
        throw std::invalid_argument(
            "The value passed as argument for chanceOfRepetition must be "
            "within the range of 0.0 - 1.0");
    }

    // TODO: may want to check that range.size is == to m_generator's vector
    // size. In fact, probably should do this for every concrete protocol that
    // has a DiscreteGenerator dependency

    // calculate the remainder allocation
    double vectorSize =
        static_cast<double>(m_generator.getDistributionVector().size());
    m_remainderAllocation = (1.0 - m_periodicity) / (vectorSize - 1.0);
}

Periodic::Periodic(IDiscreteGenerator &generator,
                   Range &range,
                   double chanceOfRepetition,
                   int initialSelection)
: Periodic(generator, range, chanceOfRepetition)
{
    if(initialSelection < range.start || initialSelection > range.end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(range.start) + " to " + std::to_string(range.end));
    }

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Periodic::~Periodic()
{}

int Periodic::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        setPeriodicDistribution(m_initialSelection - m_range.offset);
        m_haveRequestedFirstNumber = true;
        return m_initialSelection;
    }

    auto generatedNumber = m_generator.getNumber();
    setPeriodicDistribution(generatedNumber);
    return generatedNumber + m_range.offset;
}

void Periodic::reset()
{
    m_generator.updateDistributionVector(1.0);
    m_haveRequestedFirstNumber = false;
}

void Periodic::setPeriodicDistribution(int selectedIndex)
{
    // The total of all values in the vector must equal 1.0.
    // The value at the index of the last selected number
    // must have the value of the periodicity (chanceOfRepetition).
    // The remainder of 1.0 - periodicity is shared equally amongst
    // the remaining vector indices.
    auto distributionVector = m_generator.getDistributionVector();

    for(int i = 0; i < distributionVector.size(); i++) {
        auto newVectorValue =
            i == selectedIndex ? m_periodicity : m_remainderAllocation;
        distributionVector[i] = newVectorValue;
    }

    auto vectorValuesTotal = std::accumulate(distributionVector.begin(),
                                             distributionVector.end(),
                                             0.0);
    assert(vectorValuesTotal == 1.0);

    m_generator.setDistributionVector(distributionVector);
}
}} // namespace actlib::Numbers
