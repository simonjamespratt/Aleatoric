#include "Ratio.hpp"

#include <stdexcept>

namespace aleatoric {
Ratio::Ratio(std::unique_ptr<IDiscreteGenerator> generator,
             Range range,
             std::vector<int> ratios)
: m_generator(std::move(generator)),
  m_range(range),
  m_ratios(ratios),
  m_seriesPrinciple()
{
    if(m_range.size != ratios.size()) {
        throw std::invalid_argument(
            "The size of ratios collection must match the size of the range");
    }

    setSelectables();
    m_generator->setDistributionVector(m_selectables.size(), 1.0);
}

Ratio::~Ratio()
{}

int Ratio::getIntegerNumber()
{
    if(m_seriesPrinciple.seriesIsComplete(m_generator)) {
        m_seriesPrinciple.resetSeries(m_generator);
    }

    auto index = m_seriesPrinciple.getNumber(m_generator);
    return m_selectables[index];
}

double Ratio::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Ratio::reset()
{
    m_seriesPrinciple.resetSeries(m_generator);
}

void Ratio::setRange(Range newRange)
{
    // TODO: DYNAMIC-PARAMS: interim measure until we set all params at the same
    // time. At that point, replace this for a check that newRange size is same
    // as received ratios size
    if(newRange.size > m_range.size) {
        auto difference = newRange.size - m_range.size;

        for(int i = 0; i < difference; i++) {
            m_ratios.push_back(1);
        }
    }

    if(newRange.size < m_range.size) {
        auto difference = m_range.size - newRange.size;

        for(int i = 0; i < difference; i++) {
            m_ratios.pop_back();
        }
    }

    m_range = newRange;

    m_selectables.clear();
    setSelectables();
    m_generator->setDistributionVector(m_selectables.size(), 1.0);
}

Range Ratio::getRange()
{
    return m_range;
}

// Private methods
void Ratio::setSelectables()
{
    for(int i = 0; i < m_ratios.size(); i++) {
        for(int ii = 0; ii < m_ratios[i]; ii++) {
            m_selectables.push_back(m_range.offset + i);
        }
    }
}

} // namespace aleatoric
