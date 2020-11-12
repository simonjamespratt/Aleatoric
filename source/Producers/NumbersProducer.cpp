#include "NumbersProducer.hpp"

#include <stdexcept>

namespace aleatoric {
NumbersProducer::NumbersProducer(std::unique_ptr<NumberProtocol> protocol)
: m_protocol(std::move(protocol))
{}

NumbersProducer::~NumbersProducer()
{}

int NumbersProducer::getIntegerNumber()
{
    return m_protocol->getIntegerNumber();
}

double NumbersProducer::getDecimalNumber()
{
    return m_protocol->getDecimalNumber();
}

std::vector<int> NumbersProducer::getIntegerCollection(int size)
{
    std::vector<int> collection(size);

    for(auto &&it : collection) {
        it = getIntegerNumber();
    }

    return collection;
}

std::vector<double> NumbersProducer::getDecimalCollection(int size)
{
    std::vector<double> collection(size);

    for(auto &&it : collection) {
        it = getDecimalNumber();
    }

    return collection;
}

NumberProtocolParameters NumbersProducer::getParams()
{
    return m_protocol->getParams();
}

void NumbersProducer::setParams(NumberProtocolParameters newParams)
{
    if(newParams.protocols.getActiveProtocol() !=
       m_protocol->getParams().protocols.getActiveProtocol()) {
        throw std::invalid_argument(
            "Active protocol for new params is not consistent with protocol "
            "currently in use");
    }

    m_protocol->setParams(newParams);
}

void NumbersProducer::setProtocol(std::unique_ptr<NumberProtocol> protocol)
{
    m_protocol = std::move(protocol);
}

} // namespace aleatoric
