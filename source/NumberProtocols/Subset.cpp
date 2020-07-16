#include "Subset.hpp"

#include <stdexcept>

namespace aleatoric {
// NB: The original RTC version of this - choice-rhythm - does not restrict the
// size of what I am calling "subset" and it doesn't check for repetition in
// choosing the elements from the range to add to the subset. I have decided to
// change this because the RTC version can result in an unlimited collection
// with repetitions present. If you then pick random elements from that
// collection, you are effectively setting up a form of (discrete) probability
// distribution and I think we have enough ways of doing that already. I
// currently believe the interesting part of this approach is in the choosing of
// a repetition limited, subset of the range as there isn't another Protocol
// that does that. So this note is here as a reminder about the evolution of
// this away from the original should there be a desire to change it in the
// future.
Subset::Subset(std::unique_ptr<IUniformGenerator> uniformGenerator,
               std::unique_ptr<IDiscreteGenerator> discreteGenerator,
               Range range,
               int subsetMin,
               int subsetMax)
: m_uniformGenerator(std::move(uniformGenerator)),
  m_discreteGenerator(std::move(discreteGenerator)),
  m_range(range),
  m_subsetMin(subsetMin),
  m_subsetMax(subsetMax),
  m_seriesPrinciple()
{
    if(m_subsetMin < 1 || m_subsetMin > m_range.size) {
        throw std::invalid_argument(
            "The value passed as argument for subsetMin must be between 1 and "
            "the range size, inclusive");
    }

    if(m_subsetMax < 1 || m_subsetMax > m_range.size) {
        throw std::invalid_argument(
            "The value passed as argument for subsetMax must be between 1 and "
            "the range size, inclusive");
    }

    if(m_subsetMin > m_subsetMax) {
        throw std::invalid_argument("The value passed for subsetMin may not be "
                                    "greater than that of subsetMax");
    }

    m_discreteGenerator->setDistributionVector(m_range.size, 1.0);
    setSubset();
}

Subset::~Subset()
{}

int Subset::getIntegerNumber()
{
    int index = m_uniformGenerator->getNumber();
    return m_subset[index];
}

double Subset::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Subset::reset()
{
    setSubset();
}

void Subset::setRange(Range newRange)
{
    m_range = newRange;
    m_discreteGenerator->setDistributionVector(m_range.size, 1.0);
    setSubset();
    // TODO: DYNAMIC-PARAMS: there is coupling here between range and subset
    // min/max. have disregarded possibility of these two getting out of sync
    // (as per exception checks above) as they should be addressed when params
    // as a whole can be updated in next stage
}

Range Subset::getRange()
{
    return m_range;
}

// Private methods
void Subset::setSubset()
{
    m_uniformGenerator->setDistribution(m_subsetMin, m_subsetMax);

    m_subset.clear();
    m_subset.resize(m_uniformGenerator->getNumber());

    for(auto &&i : m_subset) {
        i = m_seriesPrinciple.getNumber(m_discreteGenerator) + m_range.offset;
    }

    // now set the uniformGenerator to pick indices from the subset
    m_uniformGenerator->setDistribution(0, m_subset.size() - 1);
}

} // namespace aleatoric
