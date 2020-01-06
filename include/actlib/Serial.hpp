#ifndef Serial_hpp
#define Serial_hpp

#include "IDiscreteGenerator.hpp"
#include "Protocol.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers {

/*!
 * @brief A protocol for producing random numbers
 *
 * A concrete implementation of the Protocol interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 * (see Protocol for more information).
 *
 * This protocol follows the
 * [Serialism](https://en.wikipedia.org/wiki/Serialism) approach, allowing the
 * production of sets of numbers.
 *
 * __Further Detail__: An initial call to get a number will select a number from
 * within the range with equal probability. Subsequent calls to get a number
 * will prevent previously selected numbers from being selected again until all
 * other possible numbers in the range have been selected.
 */
class Serial : public Protocol {
  public:
    /*! @brief Takes a DiscreteGenerator derived from the IDiscreteGenerator,
     * and a Range.
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
    Serial(IDiscreteGenerator &generator, Range &range);

    ~Serial();

    /*! @brief returns a random number according the the Serialism approach
     *
     * An initial call to get a number will select a number from within the
     * range with equal probability. Subsequent calls to get a number will
     * prevent previously selected numbers from being selected again until all
     * other possible numbers in the range have been selected.
     */
    int getNumber() override;

    /*! @brief will reset the distibution to the initial state of a uniform
     * distribution
     *
     * Where each number in the range has equal probability of being selected
     * upon the next call to @ref getNumber */
    void reset() override;

  private:
    Range &_range;
    IDiscreteGenerator &_generator;
    bool seriesIsComplete();
};
}} // namespace actlib::Numbers

#endif /* Serial_hpp */
