#ifndef CollectionsProducer_hpp
#define CollectionsProducer_hpp

#include "NumberProtocol.hpp"

#include <vector>

namespace aleatoric {
template<typename T>
class CollectionsProducer {
  public:
    CollectionsProducer(const std::vector<T> &source,
                        std::unique_ptr<NumberProtocol> protocol);
    ~CollectionsProducer();

    const T &getItem();
    std::vector<T> getCollection(int size);
    void reset();

  private:
    const std::vector<T> &m_source;
    std::unique_ptr<NumberProtocol> m_protocol;
};

// NB: When using templates the definitions need to be in the header or
// available to the compiler in some other way.
template<typename T>
CollectionsProducer<T>::CollectionsProducer(
    const std::vector<T> &source, std::unique_ptr<NumberProtocol> protocol)
: m_source(source), m_protocol(std::move(protocol))
{
    // TODO: DYNAMIC-PARAMS: There is no check to ensure that the protocol range
    // matches the source collection size because at present, they have no
    // knowledge of each other. So currently, it is the responsibility of the
    // client (caller) to ensure that they have configured the protocol passed
    // into this class with a range that matches the source collection size. The
    // methods in this class, getItem() and getCollection(), utilise the
    // std::vector method at() which will throw an out_of_range exception if the
    // index passed to it - which in this case is a number from the range of the
    // procotol - is outside the collection range, so in that sense there is a
    // check at the point of request. But that doesn't help the calling client
    // ensure they have their ranges in line.

    // There is some proposed work around making protocol parameters adjustable
    // at runtime (see Basecamp) and the above issue feeds into that work in the
    // sense that it will question whether a range should be adjustable at
    // runtime and whether a range should be shared. For example, in this
    // constructor, were a protocol to have a getRange() method, or were it to
    // have access to a shared pointed that is used by the protocol, then we
    // could do a check here to ensure that the ranges match.

    // The above issue should be considered at the time of working on runtime
    // protocol parameter alteration. I will update the Basecamp pitch to
    // include looking at this issue.

    // However, it might be difficult to see exactly what to do about this until
    // Aleatoric has been in use (by me!) for some time to see what
    // functionality is useful and what is not.
}

template<typename T>
CollectionsProducer<T>::~CollectionsProducer()
{}

template<typename T>
const T &CollectionsProducer<T>::getItem()
{
    // NB: using .at() because it will throw an out_of_range exception if the
    // number is out of bounds
    return m_source.at(m_protocol->getIntegerNumber());
}

template<typename T>
std::vector<T> CollectionsProducer<T>::getCollection(int size)
{
    std::vector<T> collection(size);

    for(auto &&it : collection) {
        // NB: using .at() because it will throw an out_of_range exception if
        // the number is out of bounds
        it = m_source.at(m_protocol->getIntegerNumber());
    }

    return collection;
}

template<typename T>
void CollectionsProducer<T>::reset()
{
    m_protocol->reset();
}

} // namespace aleatoric
#endif /* CollectionsProducer_hpp */
