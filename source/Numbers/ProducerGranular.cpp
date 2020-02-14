#include "ProducerGranular.hpp"

namespace actlib { namespace Numbers { namespace Granular {
Producer::Producer(Protocol &protocol) : m_protocol(protocol)
{}

Producer::~Producer()
{}

double Producer::getNumber()
{
    return m_protocol.getNumber();
}

void Producer::reset()
{
    m_protocol.reset();
}
}}} // namespace actlib::Numbers::Granular
