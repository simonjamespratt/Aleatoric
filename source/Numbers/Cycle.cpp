#include "Cycle.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers { namespace Steps {
Cycle::Cycle(std::unique_ptr<Range> range,
             bool bidirectional,
             bool reverseDirection)
: m_range(std::move(range)),
  m_bidirectional(bidirectional),
  m_reverse(reverseDirection),
  m_initialStateReverse(reverseDirection)
{
    m_reverse ? m_position = m_range->end : m_position = m_range->start;
    m_hasInitialSelection = false;
}

Cycle::Cycle(std::unique_ptr<Range> range,
             int initialSelection,
             bool bidirectional,
             bool reverseDirection)
: m_range(std::move(range)),
  m_initialSelection(initialSelection),
  m_bidirectional(bidirectional),
  m_reverse(reverseDirection),
  m_initialStateReverse(reverseDirection)
{
    if(initialSelection < m_range->start || initialSelection > m_range->end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(m_range->start) + " to " +
            std::to_string(m_range->end));
    }

    m_position = m_initialSelection;
    m_hasInitialSelection = true;
}

Cycle::~Cycle()
{}

int Cycle::getNumber()
{
    if(!m_bidirectional) {
        if(!m_reverse) {
            if(m_position == m_range->end) {
                int returnValue = m_position;
                m_position = m_range->start;
                return returnValue;
            }

            return m_position++;
        }

        if(m_reverse) {
            if(m_position == m_range->start) {
                int returnValue = m_position;
                m_position = m_range->end;
                return returnValue;
            }

            return m_position--;
        }
    }

    if(m_bidirectional) {
        if(!m_reverse) {
            if(m_position == m_range->end) {
                m_reverse = !m_reverse;
                return m_position--;
            }

            return m_position++;
        }

        if(m_reverse) {
            if(m_position == m_range->start) {
                m_reverse = !m_reverse;
                return m_position++;
            }

            return m_position--;
        }
    }
}

void Cycle::reset()
{
    if(m_bidirectional) {
        m_reverse = m_initialStateReverse;
    }

    if(m_hasInitialSelection) {
        m_position = m_initialSelection;
    } else {
        if(!m_reverse) {
            m_position = m_range->start;
        }

        if(m_reverse) {
            m_position = m_range->end;
        }
    }
}
}}} // namespace actlib::Numbers::Steps
