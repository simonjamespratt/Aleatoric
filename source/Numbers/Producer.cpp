#include "Producer.hpp"

namespace actlib { namespace Numbers {
Producer::Producer(Protocol &protocol) : _protocol(protocol) {
}

int Producer::getNumber() {
    return _protocol.getNumber();
}
}} // namespace actlib::Numbers
