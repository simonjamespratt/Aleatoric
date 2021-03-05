#include "Cycle.hpp"

#include "CycleStates.hpp"
#include "ErrorChecker.hpp"

namespace aleatoric {
Cycle::Cycle()
: m_range(0, 1),
  m_bidirectional(false),
  m_reverseDirection(false),
  m_haveRequestedFirstNumber(false)
{
    initialise();
}

Cycle::Cycle(Range range, bool bidirectional, bool reverseDirection)
: m_range(range),
  m_bidirectional(bidirectional),
  m_reverseDirection(reverseDirection),
  m_haveRequestedFirstNumber(false)
{
    initialise();
}

Cycle::~Cycle()
{}

int Cycle::getIntegerNumber()
{
    m_lastPosition = m_state->getPosition(m_nextPosition, m_range);
    m_haveRequestedFirstNumber = true;
    return m_lastPosition;
}

double Cycle::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Cycle::setParams(NumberProtocolConfig newParams)
{
    auto cycleParams = newParams.protocols.getCycle();
    m_bidirectional = cycleParams.getBidirectional();
    m_reverseDirection = cycleParams.getReverseDirection();

    setState();
    setRange(newParams.getRange());
}

NumberProtocolConfig Cycle::getParams()
{
    return NumberProtocolConfig(
        m_range,
        NumberProtocolParams(CycleParams(m_bidirectional, m_reverseDirection)));
}

// Private methods
void Cycle::setState()
{
    if(m_bidirectional) {
        m_state = std::make_unique<Bidirectional>(m_reverseDirection);
    }

    if(!m_bidirectional && m_reverseDirection) {
        m_state = std::make_unique<UniReverse>();
    }

    if(!m_bidirectional && !m_reverseDirection) {
        m_state = std::make_unique<UniForward>();
    }
}

void Cycle::setRange(Range newRange)
{
    m_range = newRange;

    m_state->setRange(m_lastPosition,
                      m_nextPosition,
                      m_range,
                      m_haveRequestedFirstNumber);
}

void Cycle::initialise()
{
    setState();

    m_reverseDirection ? m_nextPosition = m_range.end
                       : m_nextPosition = m_range.start;
}

} // namespace aleatoric
