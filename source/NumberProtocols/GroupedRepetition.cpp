#include "GroupedRepetition.hpp"

namespace aleatoric {
GroupedRepetition::GroupedRepetition(
    std::unique_ptr<IDiscreteGenerator> numberGenerator,
    std::unique_ptr<IDiscreteGenerator> groupingGenerator,
    Range range,
    std::vector<int> groupings)
: m_numberGenerator(std::move(numberGenerator)),
  m_groupingGenerator(std::move(groupingGenerator)),
  m_range(range),
  m_groupings(groupings),
  m_seriesPrinciple()
{
    m_numberGenerator->setDistributionVector(m_range.size, 1.0);
    m_groupingGenerator->setDistributionVector(m_groupings.size(), 1.0);
    m_groupingCount = 0;
}

GroupedRepetition::~GroupedRepetition()
{}

int GroupedRepetition::getIntegerNumber()
{
    if(m_seriesPrinciple.seriesIsComplete(m_groupingGenerator)) {
        m_seriesPrinciple.resetSeries(m_groupingGenerator);
    }

    if(m_seriesPrinciple.seriesIsComplete(m_numberGenerator)) {
        m_seriesPrinciple.resetSeries(m_numberGenerator);
    }

    if(m_groupingCount == 0) {
        auto groupingIndex = m_seriesPrinciple.getNumber(m_groupingGenerator);
        m_groupingCount = m_groupings[groupingIndex];

        m_currentReturnableNumber =
            m_seriesPrinciple.getNumber(m_numberGenerator) + m_range.offset;
    }

    m_groupingCount--;

    return m_currentReturnableNumber;
}

double GroupedRepetition::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void GroupedRepetition::reset()
{
    m_groupingCount = 0;
    m_seriesPrinciple.resetSeries(m_numberGenerator);
    m_seriesPrinciple.resetSeries(m_groupingGenerator);
}

void GroupedRepetition::setRange(Range newRange)
{
    m_range = newRange;
    m_numberGenerator->setDistributionVector(m_range.size, 1.0);
    m_groupingCount = 0;
}

Range GroupedRepetition::getRange()
{
    return m_range;
}

} // namespace aleatoric
