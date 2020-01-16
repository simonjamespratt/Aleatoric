#include "ProducerGranular.hpp"

namespace actlib { namespace Numbers { namespace Granular {
Producer::Producer(Protocol &protocol) : _protocol(protocol)
{}

Producer::~Producer()
{}

double Producer::getNumber()
{
    return _protocol.getNumber();
}
}}} // namespace actlib::Numbers::Granular
