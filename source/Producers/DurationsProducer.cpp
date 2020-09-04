#include "DurationsProducer.hpp"

namespace aleatoric {
DurationsProducer::DurationsProducer(
    DurationProtocol &durationProtocol,
    std::unique_ptr<NumberProtocol> numberProtocol)
: m_durationProtocol(durationProtocol),
  m_numberProtocol(std::move(numberProtocol))
{
    auto size = m_durationProtocol.getCollectionSize();
    m_numberProtocol->setRange(Range(0, size - 1));
}

DurationsProducer::~DurationsProducer()
{}

int DurationsProducer::getDuration()
{
    auto index = m_numberProtocol->getIntegerNumber();
    return m_durationProtocol.getDuration(index);
}

std::vector<int> DurationsProducer::getCollection(int size)
{
    std::vector<int> collection(size);

    for(auto &&i : collection) {
        i = getDuration();
    }

    return collection;
}

void DurationsProducer::reset()
{
    m_numberProtocol->reset();
}
} // namespace aleatoric
