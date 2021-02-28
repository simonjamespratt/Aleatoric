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

std::vector<int> DurationsProducer::getSelectableDurations()
{
    return m_durationProtocol->getSelectableDurations();
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

    notifyParamsChangeListeners();
}

int DurationsProducer::addListenerForParamsChange(
    std::function<void()> callback)
{
    if(!callback) {
        throw std::invalid_argument("Callback must not be empty");
    }

    auto id = getNewId();
    m_paramsChangeListeners.emplace(id, callback);
    return id;
}

void DurationsProducer::removeListenerForParamsChange(int callbackId)
{
    m_paramsChangeListeners.erase(callbackId);
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
        notifyParamsChangeListeners();
    }
}

// Private methods
void DurationsProducer::notifyParamsChangeListeners()
{
    for(const auto &item : m_paramsChangeListeners) {
        auto callback = item.second;
        if(callback) {
            callback();
        }
    }
}

int DurationsProducer::getNewId()
{
    return ++m_listenersIdCounter;
}

} // namespace aleatoric
