#ifndef Producer_hpp
#define Producer_hpp

#include "Protocol.hpp"

namespace actlib { namespace Numbers {

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
    Producer(Protocol &protocol);

    ~Producer();

    /*! @brief Returns a random number created according to the selected
     * protocol */
    int getNumber();

  private:
    Protocol &_protocol;
};
}} // namespace actlib::Numbers

#endif /* Producer_hpp */
