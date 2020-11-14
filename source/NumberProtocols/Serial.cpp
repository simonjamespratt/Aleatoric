#include "Serial.hpp"

#include "SeriesPrinciple.hpp"

namespace aleatoric {
Serial::Serial(std::unique_ptr<IDiscreteGenerator> generator)
: m_generator(std::move(generator)),
  m_range(0, 1),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

Serial::Serial(std::unique_ptr<IDiscreteGenerator> generator, Range range)
: m_generator(std::move(generator)),
  m_range(range),
  m_seriesPrinciple(std::make_unique<SeriesPrinciple>())
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

Serial::~Serial()
{}

int Serial::getIntegerNumber()
{
    if(m_seriesPrinciple->seriesIsComplete(m_generator)) {
        m_seriesPrinciple->resetSeries(m_generator);
    }

    int selectedNumber = m_seriesPrinciple->getNumber(m_generator);
    return selectedNumber + m_range.offset;
}

double Serial::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Serial::setParams(NumberProtocolParameters newParams)
{
    m_range = newParams.getRange();
    m_generator->setDistributionVector(m_range.size, 1.0);
}

NumberProtocolParameters Serial::getParams()
{
    return NumberProtocolParameters(m_range,
                                    NumberProtocolParameters::Protocols(
                                        NumberProtocolParameters::Serial()));
}

} // namespace aleatoric
