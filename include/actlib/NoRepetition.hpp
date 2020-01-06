#ifndef NoRepetition_hpp
#define NoRepetition_hpp

#include "IDiscreteGenerator.hpp"
#include "Protocol.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers {

/*! @brief A protocol for producing random numbers
 *
 * A concrete implementation of the Protocol interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 * (see Protocol for more information).
 *
 * A variation on producing random numbers where a constraint is placed on
 * production that disallows the _last selected_ number from being selected upon
 * the next call to get a number. As such, the protocol prevents _immediate
 * repetition_ of numbers.
 *
 * __Further Detail__: An initial call to get a number will select a number from
 * within the range with equal probability. The next call to get a number will
 * prevent this number from being selected, whilst all other numbers in the
 * range have an equal probability of being selected.
 */
class NoRepetition : public Protocol {
  public:
    /*! @brief Takes a DiscreteGenerator derived from the IDiscreteGenerator,
     * and a Range
     *
     * @param generator should be an instance of DiscreteGenerator that has been
     * instantiated with an Engine, a vectorSize and a uniformValue. For
     * example:
     *
     *      DiscreteGenerator generator(engine, vectorSize, uniformValue)
     *
     * An instance of the generator initialised as above will provide an initial
     * state of a uniform distribution and thus equal probability of selection
     * for each number in the range.
     *
     * @param range should map to the vectorSize argument supplied to the
     * DiscreteGenerator. In other words, range.size should equal vectorSize
     */
    NoRepetition(IDiscreteGenerator &generator, Range &range);

    ~NoRepetition();

    /*! @brief returns a random number preventing immediate repetition of the
     * last selected number
     *
     * An initial call to get a number will select a number from within
     * the range with equal probability. The next call to get a number will
     * prevent this number from being selected, whilst all other numbers in the
     * range have an equal probability of being selected.
     */
    int getNumber() override;

    /*! @brief will reset the distibution to the initial state of a uniform
     * distribution
     *
     * Where each number in the range has equal probability of being selected
     * upon the next call to @ref getNumber */
    void reset() override;

  private:
    actlib::Numbers::Range &_range;
    actlib::Numbers::IDiscreteGenerator &_generator;
};
}} // namespace actlib::Numbers

#endif /* NoRepetition_hpp */
