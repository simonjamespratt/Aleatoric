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
    m_protocol->setRange(Range(0, m_source.size() - 1));
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
