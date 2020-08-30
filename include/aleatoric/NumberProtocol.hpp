#ifndef NumberProtocol_hpp
#define NumberProtocol_hpp

#include "NumberProtocolParameters.hpp"
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

    virtual void setParams(NumberProtocolParameters newParams) = 0;

    virtual NumberProtocolParameters getParams() = 0;

    virtual ~NumberProtocol() = default;

    enum class Type {
        adjacentSteps,
        basic,
        cycle,
        granularWalk,
        groupedRepetition,
        noRepetition,
        periodic,
        precision,
        ratio,
        serial,
        subset,
        walk
    };

    static std::unique_ptr<NumberProtocol> create(Type type);
};
} // namespace aleatoric

#endif /* NumberProtocol_hpp */
