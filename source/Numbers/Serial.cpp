#include "Serial.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Serial::Serial(std::unique_ptr<IDiscreteGenerator> generator,
               std::unique_ptr<Range> range)
: m_range(std::move(range)),
  m_generator(std::move(generator)),
  m_seriesPrinciple()
{
    m_generator->setDistributionVector(m_range->size, 1.0);
}

Serial::~Serial()
{}

int Serial::getNumber()
{
    if(m_seriesPrinciple.seriesIsComplete(m_generator)) {
        m_seriesPrinciple.resetSeries(m_generator);
    }

    int selectedNumber = m_seriesPrinciple.getNumber(m_generator);
    return selectedNumber + m_range->offset;
}

void Serial::reset()
{
    m_seriesPrinciple.resetSeries(m_generator);
}

}}} // namespace actlib::Numbers::Steps
