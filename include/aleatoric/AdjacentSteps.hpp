#ifndef AdjacentSteps_hpp
#define AdjacentSteps_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

namespace aleatoric {

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
class AdjacentSteps : public NumberProtocol {
  public:
    AdjacentSteps(std::unique_ptr<IDiscreteGenerator> generator);

    /*! @brief Takes a DiscreteGenerator derived from the IDiscreteGenerator,
     * and a Range
     *
     * @param generator Should be an instance of DiscreteGenerator. Default
     * construction is fine.
     *
     * @param range The range within which to producde numbers.
     */
    AdjacentSteps(std::unique_ptr<IDiscreteGenerator> generator, Range range);

    ~AdjacentSteps();

    /*! @brief Returns a number according to the protocol.
     *
     * See the detailed description for more information.
     */
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

    /*! @brief Resets the state of the class to the state upon instantiation
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
    Range m_range;
    std::unique_ptr<IDiscreteGenerator> m_generator;
    void prepareStepBasedDistribution(int number);
    bool m_haveRequestedFirstNumber;
    int m_lastReturnedNumber;
};
} // namespace aleatoric

#endif /* AdjacentSteps_hpp */
