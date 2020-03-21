#include "ProducerSteps.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Producer::Producer(std::unique_ptr<Protocol> protocol)
: m_protocol(std::move(protocol))
{}

Producer::~Producer()
{}

int Producer::getNumber()
{
    return m_protocol->getNumber();
}

void Producer::reset()
{
    m_protocol->reset();
}

}}} // namespace actlib::Numbers::Steps
