#ifndef CollectionsProducer_hpp
#define CollectionsProducer_hpp

#include "NumberProtocol.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace aleatoric {
template<typename T>
class CollectionsProducer {
  public:
    CollectionsProducer(std::vector<T> source,
                        std::unique_ptr<NumberProtocol> protocol);
    ~CollectionsProducer();

    const T &getItem();
    std::vector<T> getCollection(int size);
    NumberProtocolParameters::Protocols getParams();
    void setParams(NumberProtocolParameters::Protocols newParams);
    void setProtocol(std::unique_ptr<NumberProtocol> protocol);
    void setSource(std::vector<T> newSource);
    std::vector<T> getSource();

  private:
    std::vector<T> m_source;
    std::unique_ptr<NumberProtocol> m_protocol;
};

// NB: When using templates the definitions need to be in the header or
// available to the compiler in some other way.
template<typename T>
CollectionsProducer<T>::CollectionsProducer(
    std::vector<T> source, std::unique_ptr<NumberProtocol> protocol)
: m_source(source), m_protocol(std::move(protocol))
{
    try {
        m_protocol->setParams(Range(0, m_source.size() - 1));
    } catch(const std::exception &e) {
        throw std::invalid_argument(
            "The size of the source collection provided is too small. It must "
            "be two or greater");
    }
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
        it = getItem();
    }

    return collection;
}

template<typename T>
NumberProtocolParameters::Protocols CollectionsProducer<T>::getParams()
{
    return m_protocol->getParams().protocols;
}

template<typename T>
void CollectionsProducer<T>::setParams(
    NumberProtocolParameters::Protocols newParams)
{
    if(newParams.getActiveProtocol() !=
       m_protocol->getParams().protocols.getActiveProtocol()) {
        throw std::invalid_argument(
            "Active protocol for new params is not consistent with protocol "
            "currently in use");
    }

    m_protocol->setParams(
        NumberProtocolParameters(Range(0, m_source.size() - 1), newParams));
}

template<typename T>
void CollectionsProducer<T>::setProtocol(
    std::unique_ptr<NumberProtocol> protocol)
{
    m_protocol = std::move(protocol);
    m_protocol->setParams(Range(0, m_source.size() - 1));
}

template<typename T>
void CollectionsProducer<T>::setSource(std::vector<T> newSource)
{
    if(newSource.size() != m_source.size()) {
        try {
            m_protocol->setParams(Range(0, newSource.size() - 1));
        } catch(const std::exception &e) {
            throw std::invalid_argument(
                "The size of the source collection provided is too small. It "
                "must be two or greater");
        }
    }

    m_source = newSource;
}

template<typename T>
std::vector<T> CollectionsProducer<T>::getSource()
{
    return m_source;
}

} // namespace aleatoric
#endif /* CollectionsProducer_hpp */
