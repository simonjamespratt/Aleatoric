#include "Producer.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Producer::Producer(Protocol &protocol) : _protocol(protocol)
{}

Producer::~Producer()
{}

int Producer::getNumber()
{
    return _protocol.getNumber();
}
}}} // namespace actlib::Numbers::Steps
