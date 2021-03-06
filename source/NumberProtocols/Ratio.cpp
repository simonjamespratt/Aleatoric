#include "Ratio.hpp"

#include "SeriesPrinciple.hpp"

#include <stdexcept>

namespace aleatoric {
Ratio::Ratio(std::unique_ptr<IDiscreteGenerator> generator)
: m_generator(std::move(generator)),
  m_range(0, 1),
  m_ratios(std::vector<int> {1, 1}),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    initialise();
}

Ratio::Ratio(std::unique_ptr<IDiscreteGenerator> generator,
             Range range,
             std::vector<int> ratios)
: m_generator(std::move(generator)),
  m_range(range),
  m_ratios(ratios),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    checkRangeAndRatiosMatch(m_range, m_ratios);
    initialise();
}

Ratio::~Ratio()
{}

int Ratio::getIntegerNumber()
{
    if(m_seriesPrinciple->seriesIsComplete(m_generator)) {
        m_seriesPrinciple->resetSeries(m_generator);
    }

    auto index = m_seriesPrinciple->getNumber(m_generator);
    return m_selectables[index];
}

double Ratio::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Ratio::setParams(NumberProtocolConfig newParams)
{
    auto newRatios = newParams.protocols.getRatio().getRatios();
    auto newRange = newParams.getRange();
    checkRangeAndRatiosMatch(newRange, newRatios);
    m_ratios = newRatios;
    m_range = newRange;
    m_selectables.clear();
    setSelectables();
    m_generator->setDistributionVector(m_selectables.size(), 1.0);
}

NumberProtocolConfig Ratio::getParams()
{
    return NumberProtocolConfig(m_range,
                                NumberProtocolParams(RatioParams(m_ratios)));
}

// Private methods
void Ratio::setSelectables()
{
    for(size_t i = 0; i < m_ratios.size(); i++) {
        for(int ii = 0; ii < m_ratios[i]; ii++) {
            m_selectables.push_back(m_range.offset + i);
        }
    }
}

void Ratio::checkRangeAndRatiosMatch(const Range &range,
                                     const std::vector<int> &ratios)
{
    if(range.size != static_cast<int>(ratios.size())) {
        throw std::invalid_argument(
            "The size of ratios collection must match the size of the range");
    }
}

void Ratio::initialise()
{
    setSelectables();
    m_generator->setDistributionVector(m_selectables.size(), 1.0);
}

} // namespace aleatoric
