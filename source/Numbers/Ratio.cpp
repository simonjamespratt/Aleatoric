#include "Ratio.hpp"

#include <stdexcept>

namespace actlib { namespace Numbers { namespace Steps {
Ratio::Ratio(std::unique_ptr<IDiscreteGenerator> generator,
             std::unique_ptr<Range> range,
             std::vector<int> ratios)
: m_generator(std::move(generator)),
  m_range(std::move(range)),
  m_seriesPrinciple()
{
    if(m_range->size != ratios.size()) {
        throw std::invalid_argument(
            "The size of ratios collection must match the size of the range");
    }

    for(int i = 0; i < ratios.size(); i++) {
        for(int ii = 0; ii < ratios[i]; ii++) {
            m_selectables.push_back(m_range->offset + i);
        }
    }

    m_generator->setDistributionVector(m_selectables.size(), 1.0);
}

Ratio::~Ratio()
{}

int Ratio::getNumber()
{
    if(m_seriesPrinciple.seriesIsComplete(m_generator)) {
        m_seriesPrinciple.resetSeries(m_generator);
    }

    auto index = m_seriesPrinciple.getNumber(m_generator);
    return m_selectables[index];
}

void Ratio::reset()
{
    m_seriesPrinciple.resetSeries(m_generator);
}

}}} // namespace actlib::Numbers::Steps
