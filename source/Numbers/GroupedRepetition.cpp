#include "GroupedRepetition.hpp"

namespace actlib { namespace Numbers { namespace Steps {
GroupedRepetition::GroupedRepetition(
    std::unique_ptr<IDiscreteGenerator> numberGenerator,
    std::unique_ptr<IDiscreteGenerator> groupingGenerator,
    std::unique_ptr<Range> range,
    std::vector<int> groupings)
: m_numberGenerator(std::move(numberGenerator)),
  m_groupingGenerator(std::move(groupingGenerator)),
  m_range(std::move(range)),
  m_groupings(groupings),
  m_seriesPrinciple()
{
    m_numberGenerator->setDistributionVector(m_range->size, 1.0);
    m_groupingGenerator->setDistributionVector(m_groupings.size(), 1.0);
    m_groupingCount = 0;
}

GroupedRepetition::~GroupedRepetition()
{}

int GroupedRepetition::getNumber()
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
            m_seriesPrinciple.getNumber(m_numberGenerator) + m_range->offset;
    }

    m_groupingCount--;

    return m_currentReturnableNumber;
}

void GroupedRepetition::reset()
{
    m_groupingCount = 0;
    m_seriesPrinciple.resetSeries(m_numberGenerator);
    m_seriesPrinciple.resetSeries(m_groupingGenerator);
}

}}} // namespace actlib::Numbers::Steps
