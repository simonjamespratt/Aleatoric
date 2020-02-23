#include "ProducerSteps.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Producer::Producer(Protocol &protocol) : m_protocol(protocol)
{}

Producer::~Producer()
{}

int Producer::getNumber()
{
    return m_protocol.getNumber();
}

void Producer::reset()
{
    m_protocol.reset();
}

}}} // namespace actlib::Numbers::Steps
