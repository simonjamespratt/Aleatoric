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

std::vector<int> Producer::getCollection(int size)
{
    std::vector<int> collection(size);

    for(auto &&it : collection) {
        it = m_protocol->getNumber();
    }

    return collection;
}

void Producer::reset()
{
    m_protocol->reset();
}

}}} // namespace actlib::Numbers::Steps
