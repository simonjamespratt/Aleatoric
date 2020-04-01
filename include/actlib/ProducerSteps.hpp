#ifndef ProducerSteps_hpp
#define ProducerSteps_hpp

#include "ProtocolSteps.hpp"

#include <memory>
#include <vector>

namespace actlib { namespace Numbers { namespace Steps {

/*! @brief Context in which to select a protocol to use for producing random
 * numbers
 *
 * It is the _context_ which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern.
 *
 * This class is configured with a concrete protocol strategy and maintains a
 * reference to it via the Protocol interface.
 */
class Producer {
  public:
    /*! @brief Constructor taking a reference to a protocol
     *
     * @param protocol a concrete protocol strategy, passed by reference to the
     * Procotol interface
     */
    // NB: This takes ownership because the results of letting it take the
    // protocol by reference, and therefore potentially allowing use of the
    // protocol in different Producers, are unknown, as internal state of a
    // Protocol may be different from that expected, having potentially been
    // used by another Producer
    Producer(std::unique_ptr<Protocol> protocol);

    ~Producer();

    /*! @brief Returns a random number created according to the selected
     * protocol */
    int getNumber();

    std::vector<int> getCollection(int size);

    /*!
     * @brief Resets the protocol being used
     */
    void reset();

  private:
    std::unique_ptr<Protocol> m_protocol;
};
}}} // namespace actlib::Numbers::Steps

#endif /* ProducerSteps_hpp */
