#ifndef ProducerGranular_hpp
#define ProducerGranular_hpp

#include "ProtocolGranular.hpp"

#include <memory>
#include <vector>

namespace actlib { namespace Numbers { namespace Granular {

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
    Producer(std::unique_ptr<Protocol> protocol);

    ~Producer();

    /*! @brief Returns a random number created according to the selected
     * protocol */
    double getNumber();

    std::vector<double> getCollection(int size);

    /*!
     * @brief Resets the protocol being used
     */
    void reset();

  private:
    std::unique_ptr<Protocol> m_protocol;
};
}}} // namespace actlib::Numbers::Granular

#endif /* ProducerGranular_hpp */
