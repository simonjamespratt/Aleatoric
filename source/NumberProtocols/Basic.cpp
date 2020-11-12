#include "Basic.hpp"

namespace aleatoric {
Basic::Basic(std::unique_ptr<IUniformGenerator> generator)
: m_generator(std::move(generator)), m_range(0, 1)
{
    m_generator->setDistribution(m_range.start, m_range.end);
}

Basic::Basic(std::unique_ptr<IUniformGenerator> generator, Range range)
: m_generator(std::move(generator)), m_range(range)
{
    m_generator->setDistribution(m_range.start, m_range.end);
}

Basic::~Basic()
{}

int Basic::getIntegerNumber()
{
    return m_generator->getNumber();
}

double Basic::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

NumberProtocolParameters Basic::getParams()
{
    return NumberProtocolParameters(
        m_range,
        NumberProtocolParameters::Protocols(NumberProtocolParameters::Basic()));
}

void Basic::setParams(NumberProtocolParameters newParams)
{
    m_range = newParams.getRange();
    m_generator->setDistribution(m_range.start, m_range.end);
}
} // namespace aleatoric
