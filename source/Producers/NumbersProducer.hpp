#ifndef NumbersProducer_hpp
#define NumbersProducer_hpp

#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <memory>
#include <vector>

namespace aleatoric {

/*! @brief Context in which to select a protocol to use for producing random
 * numbers
 *
 * It is the _context_ which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern.
 *
 * This class is configured with a concrete protocol strategy and maintains a
 * reference to it via the Protocol interface.
 */
class NumbersProducer {
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
    NumbersProducer(std::unique_ptr<NumberProtocol> protocol);

    ~NumbersProducer();

    /*! @brief Returns a random number created according to the selected
     * protocol */
    int getIntegerNumber();

    double getDecimalNumber();

    std::vector<int> getIntegerCollection(int size);

    std::vector<double> getDecimalCollection(int size);

    NumberProtocolConfig getParams();

    void setParams(NumberProtocolConfig newParams);

    void setProtocol(std::unique_ptr<NumberProtocol> protocol);

  private:
    std::unique_ptr<NumberProtocol> m_protocol;
};
} // namespace aleatoric

#endif /* NumbersProducer_hpp */
