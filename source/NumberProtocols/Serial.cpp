#include "Serial.hpp"

namespace aleatoric {
Serial::Serial(std::unique_ptr<IDiscreteGenerator> generator, Range range)
: m_range(range), m_generator(std::move(generator)), m_seriesPrinciple()
{
    m_generator->setDistributionVector(m_range.size, 1.0);
}

Serial::~Serial()
{}

int Serial::getIntegerNumber()
{
    if(m_seriesPrinciple.seriesIsComplete(m_generator)) {
        m_seriesPrinciple.resetSeries(m_generator);
    }

    int selectedNumber = m_seriesPrinciple.getNumber(m_generator);
    return selectedNumber + m_range.offset;
}

double Serial::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Serial::reset()
{
    m_seriesPrinciple.resetSeries(m_generator);
}

void Serial::setRange(Range newRange)
{
    m_range = newRange;
    m_generator->setDistributionVector(m_range.size, 1.0);
}

Range Serial::getRange()
{
    return m_range;
}

} // namespace aleatoric
