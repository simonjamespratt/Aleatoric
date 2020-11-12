#include "DurationsProducer.hpp"

#include <stdexcept>

namespace aleatoric {
DurationsProducer::DurationsProducer(
    std::unique_ptr<DurationProtocol> durationProtocol,
    std::unique_ptr<NumberProtocol> numberProtocol)
: m_durationProtocol(std::move(durationProtocol)),
  m_numberProtocol(std::move(numberProtocol))
{
    m_durationCollectionSize = m_durationProtocol->getCollectionSize();
    m_numberProtocol->setParams(Range(0, m_durationCollectionSize - 1));
}

DurationsProducer::~DurationsProducer()
{}

int DurationsProducer::getDuration()
{
    auto index = m_numberProtocol->getIntegerNumber();
    return m_durationProtocol->getDuration(index);
}

std::vector<int> DurationsProducer::getCollection(int size)
{
    std::vector<int> collection(size);

    for(auto &&i : collection) {
        i = getDuration();
    }

    return collection;
}

NumberProtocolParameters::Protocols DurationsProducer::getParams()
{
    return m_numberProtocol->getParams().protocols;
}

void DurationsProducer::setParams(NumberProtocolParameters::Protocols newParams)
{
    if(newParams.getActiveProtocol() !=
       m_numberProtocol->getParams().protocols.getActiveProtocol()) {
        throw std::invalid_argument(
            "Active protocol for new params is not consistent with protocol "
            "currently in use");
    }

    m_numberProtocol->setParams(
        NumberProtocolParameters(Range(0, m_durationCollectionSize - 1),
                                 newParams));
}

void DurationsProducer::setNumberProtocol(
    std::unique_ptr<NumberProtocol> numberProtocol)
{
    m_numberProtocol = std::move(numberProtocol);
}

void DurationsProducer::setDurationProtocol(
    std::unique_ptr<DurationProtocol> durationProtocol)
{
    m_durationProtocol = std::move(durationProtocol);

    auto newCollectionSize = m_durationProtocol->getCollectionSize();

    if(newCollectionSize != m_durationCollectionSize) {
        m_durationCollectionSize = newCollectionSize;
        m_numberProtocol->setParams(Range(0, m_durationCollectionSize - 1));
    }
}

} // namespace aleatoric
