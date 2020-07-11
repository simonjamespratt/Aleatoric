#include "DurationsProducer.hpp"

namespace aleatoric {
DurationsProducer::DurationsProducer(
    DurationProtocol &durationProtocol,
    std::unique_ptr<NumberProtocol> numberProtocol)
: m_durationProtocol(durationProtocol),
  m_numberProtocol(std::move(numberProtocol))
{
    // TODO: DYNAMIC-PARAMS: There is no check to ensure that the number
    // protocol range matches the duration protocol collection size. This is
    // very similar to the issue in the collections producer. As it stands, all
    // concrete implementations of the duration protocol method getDuration()
    // are accessing an item in a vector by index through vector.at() which will
    // throw an out of bounds exception if that index is inaccessible, so this
    // is also similar to the over all outcome that occurs in the collections
    // producer.

    // What isn't documented in the collection producer note about this is that
    // both of these producers could do a check themselves to ensure that the
    // number returned by the numbers protocol (that is then used to access an
    // index) is within the bounds of the collection. This could be done without
    // the other work described in the collections producer todo.

    // However, it would be better if a way was found to ensure that the numbers
    // produced by the number protocols are, and stay, in line with the
    // collection they are used to select items from, thereby avoiding an
    // exception altogether.
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
