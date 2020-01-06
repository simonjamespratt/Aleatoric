#ifndef AdjacentSteps_hpp
#define AdjacentSteps_hpp

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
 * Places a constraint on the production of numbers where only numbers
 * __adjacent__ to the _last seleted_ number in a range can be selected. As such
 * this forms a stepwise traversal of the range.
 *
 * __Further Detail__:
 *
 * Having made an initial call to get a number, a subsequent
 * call for a number will only allow the selection of the number _directly_
 * above or below the previously selected number in the range.
 *
 * Having selected either the number above or below, a further call for a number
 * will only allow the selection of the number _directly_ above or below _that_
 * last number selection. And so on and so on.
 *
 * Note that traversal of the range does _not_ wrap. In other words, when either
 * end of the range is reached, only the a step in the _opposite_ direction is
 * allowed.
 *
 * Upon instantiating the protocol, the initial number to be selected can be
 * provided to the constructor. This will be the starting point for the
 * traversal of the range and the number returned upon the first call for a
 * number. If an initial number is not provided, the first call to get a number
 * will pick one from the range at random (equal probability / uniform
 * distribution).
 */
class AdjacentSteps : public Protocol {
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
     * @param range should map to the vectorSize argument supplied tothe
     * DiscreteGenerator. In other words, range.size should equal vectorSize
     */
    AdjacentSteps(IDiscreteGenerator &generator, Range &range);

    /*! @overload
     *
     * Accepts an additional parameter for setting the initial number to be
     * selected.
     *
     * @param initialSelection this number will be returned on the first call to
     * getNumber() and will be the starting point for the range traversal from
     * thereon
     */
    AdjacentSteps(IDiscreteGenerator &generator,
                  Range &range,
                  int initialSelection);

    ~AdjacentSteps();

    /*! @brief Returns a number according to the protocol.
     *
     * See the detailed description for more information.
     */
    int getNumber() override;

    /*! @brief Resets the  state of the class to the state upon instantiation
     *
     * This means that if the class was instantiated with no initial number
     * selection, upon the next call to getNumber(), a number will be selected
     * at random with equal probability of selection from which to start the
     * range traversal.
     *
     * If the class was instantiated with an initial number selection, this will
     * be the number returned from the next call to getNumber(), and will form
     * the starting point for the range traversal from thereon.
     */
    void reset() override;

  private:
    Range &m_range;
    IDiscreteGenerator &m_generator;
    void prepareStepBasedDistribution(int numberPlacedWithinRange,
                                      int generatedNumber);
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
}} // namespace actlib::Numbers

#endif /* AdjacentSteps_hpp */
