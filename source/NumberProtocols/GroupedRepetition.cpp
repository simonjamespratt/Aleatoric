#include "GroupedRepetition.hpp"

#include "SeriesPrinciple.hpp"

namespace aleatoric {
GroupedRepetition::GroupedRepetition(
    std::unique_ptr<IDiscreteGenerator> numberGenerator,
    std::unique_ptr<IDiscreteGenerator> groupingGenerator)
: m_numberGenerator(std::move(numberGenerator)),
  m_groupingGenerator(std::move(groupingGenerator)),
  m_range(0, 1),
  m_groupings({1}),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    initialise();
}

GroupedRepetition::GroupedRepetition(
    std::unique_ptr<IDiscreteGenerator> numberGenerator,
    std::unique_ptr<IDiscreteGenerator> groupingGenerator,
    Range range,
    std::vector<int> groupings)
: m_numberGenerator(std::move(numberGenerator)),
  m_groupingGenerator(std::move(groupingGenerator)),
  m_range(range),
  m_groupings(groupings),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    initialise();
}

GroupedRepetition::~GroupedRepetition()
{}

int GroupedRepetition::getIntegerNumber()
{
    if(m_seriesPrinciple->seriesIsComplete(m_groupingGenerator)) {
        m_seriesPrinciple->resetSeries(m_groupingGenerator);
    }

    if(m_seriesPrinciple->seriesIsComplete(m_numberGenerator)) {
        m_seriesPrinciple->resetSeries(m_numberGenerator);
    }

    if(m_groupingCount == 0) {
        auto groupingIndex = m_seriesPrinciple->getNumber(m_groupingGenerator);
        m_groupingCount = m_groupings[groupingIndex];

        m_currentReturnableNumber =
            m_seriesPrinciple->getNumber(m_numberGenerator) + m_range.offset;
    }

    m_groupingCount--;

    return m_currentReturnableNumber;
}

double GroupedRepetition::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void GroupedRepetition::setParams(NumberProtocolConfig newParams)
{
    m_groupings = newParams.protocols.getGroupedRepetition().getGroupings();
    m_groupingGenerator->setDistributionVector(m_groupings.size(), 1.0);

    m_range = newParams.getRange();
    m_numberGenerator->setDistributionVector(m_range.size, 1.0);

    m_groupingCount = 0;
}

NumberProtocolConfig GroupedRepetition::getParams()
{
    return NumberProtocolConfig(
        m_range,
        NumberProtocolParams(GroupedRepetitionParams(m_groupings)));
}

// Private methods
void GroupedRepetition::initialise()
{
    m_numberGenerator->setDistributionVector(m_range.size, 1.0);
    m_groupingGenerator->setDistributionVector(m_groupings.size(), 1.0);
    m_groupingCount = 0;
}

} // namespace aleatoric
