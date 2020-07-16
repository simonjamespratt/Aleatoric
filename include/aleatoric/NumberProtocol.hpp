#ifndef NumberProtocol_hpp
#define NumberProtocol_hpp

#include "Range.hpp"

namespace aleatoric {

/*! @brief Interface to which concrete protocol classes that produce random
 * numbers must conform
 *
 * It is the _strategy_ interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 *
 * The Producer class (_context_ in the strategy pattern) holds a reference to
 * concrete protocols via this interface.
 */
class NumberProtocol {
  public:
    /*! Pure virtual method for getting random numbers from a protocol */
    virtual int getIntegerNumber() = 0;

    virtual double getDecimalNumber() = 0;

    /*! Pure virtual method for resetting a protocol to its initial state */
    virtual void reset() = 0;

    virtual void setRange(Range newRange) = 0;

    virtual Range getRange() = 0;

    virtual ~NumberProtocol() = default;
};
} // namespace aleatoric

#endif /* NumberProtocol_hpp */
