#include "Cycle.hpp"

#include "ErrorChecker.hpp"

namespace aleatoric {
Cycle::Cycle(Range range, bool bidirectional, bool reverseDirection)
: m_range(range), m_haveRequestedFirstNumber(false)
{
    setState(bidirectional, reverseDirection);

    reverseDirection ? m_nextPosition = m_range.end
                     : m_nextPosition = m_range.start;
}

Cycle::~Cycle()
{}

int Cycle::getIntegerNumber()
{
    auto number = m_state->getPosition(m_nextPosition, m_range);
    m_haveRequestedFirstNumber = true;
    return number;
}

double Cycle::getDecimalNumber()
{
    return static_cast<double>(getIntegerNumber());
}

void Cycle::reset()
{
    m_state->reset(m_nextPosition, m_range);
}

void Cycle::setRange(Range newRange)
{
    m_range = newRange;

    m_state->setRange(m_nextPosition, m_range, m_haveRequestedFirstNumber);
}

Range Cycle::getRange()
{
    return m_range;
}

void Cycle::setState(bool bidirectional, bool reverseDirection)
{
    if(bidirectional) {
        m_state = std::make_unique<Bidirectional>(reverseDirection);
    }

    if(!bidirectional && reverseDirection) {
        m_state = std::make_unique<UniReverse>();
    }

    if(!bidirectional && !reverseDirection) {
        m_state = std::make_unique<UniForward>();
    }
}

} // namespace aleatoric
