#include "CycleStates.hpp"

namespace aleatoric {
UniForward::UniForward()
{}
int UniForward::getPosition(int &nextPosition, const Range &range)
{
    if(nextPosition == range.end) {
        int returnValue = nextPosition;
        nextPosition = range.start;
        return returnValue;
    }

    return nextPosition++;
}

void UniForward::reset(int &nextPosition, const Range &range)
{
    nextPosition = range.start;
}

void UniForward::setRange(const int &lastPosition,
                          int &nextPosition,
                          const Range &range,
                          const bool &haveRequestedFirstNumber)
{
    nextPosition = lastPosition + 1;

    if(lastPosition == range.end || !range.numberIsInRange(lastPosition) ||
       !haveRequestedFirstNumber) {
        nextPosition = range.start;
    }
}

UniReverse::UniReverse()
{}
int UniReverse::getPosition(int &nextPosition, const Range &range)
{
    if(nextPosition == range.start) {
        int returnValue = nextPosition;
        nextPosition = range.end;
        return returnValue;
    }

    return nextPosition--;
}

void UniReverse::reset(int &nextPosition, const Range &range)
{
    nextPosition = range.end;
}

void UniReverse::setRange(const int &lastPosition,
                          int &nextPosition,
                          const Range &range,
                          const bool &haveRequestedFirstNumber)
{
    nextPosition = lastPosition - 1;

    if(!range.numberIsInRange(lastPosition) || lastPosition == range.start ||
       !haveRequestedFirstNumber) {
        nextPosition = range.end;
    }
}

Bidirectional::Bidirectional(bool initialStateReverse)
: m_initialStateReverse(initialStateReverse), m_reverse(initialStateReverse)
{}

int Bidirectional::getPosition(int &nextPosition, const Range &range)
{
    if(nextPosition == range.end && !m_reverse) {
        m_reverse = !m_reverse;
        return nextPosition--;
    }

    if(nextPosition == range.start && m_reverse) {
        m_reverse = !m_reverse;
        return nextPosition++;
    }

    return m_reverse ? nextPosition-- : nextPosition++;
}

void Bidirectional::reset(int &nextPosition, const Range &range)
{
    m_reverse = m_initialStateReverse;
    nextPosition = m_reverse ? range.end : range.start;
}

void Bidirectional::setRange(const int &lastPosition,
                             int &nextPosition,
                             const Range &range,
                             const bool &haveRequestedFirstNumber)
{
    nextPosition = m_reverse ? lastPosition - 1 : lastPosition + 1;

    if(haveRequestedFirstNumber && lastPosition == range.end) {
        nextPosition = range.end - 1;
        m_reverse = true;
    }

    if(haveRequestedFirstNumber && lastPosition == range.start) {
        nextPosition = range.start + 1;
        m_reverse = false;
    }

    if(!range.numberIsInRange(lastPosition) || !haveRequestedFirstNumber) {
        nextPosition = m_reverse ? range.end : range.start;
    }
}
} // namespace aleatoric
