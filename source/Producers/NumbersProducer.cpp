#include "NumbersProducer.hpp"

namespace aleatoric {
NumbersProducer::NumbersProducer(std::unique_ptr<NumberProtocol> protocol,
                                 Range range)
: m_protocol(std::move(protocol))
{
    m_protocol->setRange(range);
}

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
        it = m_protocol->getIntegerNumber();
    }

    return collection;
}

std::vector<double> NumbersProducer::getDecimalCollection(int size)
{
    std::vector<double> collection(size);

    for(auto &&it : collection) {
        it = m_protocol->getDecimalNumber();
    }

    return collection;
}

void NumbersProducer::reset()
{
    m_protocol->reset();
}

} // namespace aleatoric
