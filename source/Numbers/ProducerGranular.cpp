#include "ProducerGranular.hpp"

namespace actlib { namespace Numbers { namespace Granular {
Producer::Producer(std::unique_ptr<Protocol> protocol)
: m_protocol(std::move(protocol))
{}

Producer::~Producer()
{}

double Producer::getNumber()
{
    return m_protocol->getNumber();
}

std::vector<double> Producer::getCollection(int size)
{
    std::vector<double> collection(size);

    for(auto &&it : collection) {
        it = m_protocol->getNumber();
    }

    return collection;
}

void Producer::reset()
{
    m_protocol->reset();
}
}}} // namespace actlib::Numbers::Granular
