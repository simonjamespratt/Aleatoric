#ifndef Periodic_hpp
#define Periodic_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers { namespace Steps {

/*! @brief A protocol for producing random numbers
 *
 * A concrete implementation of the Protocol interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 * (see Protocol for more information).
 *
 * Introduces a bias on the prodction of numbers where the _last selected_
 * number can be repeated when the next number is requested, according to a
 * "periodicity".
 *
 * The periodicity is a double between 0 and 1 which describes a
 * _likelihood_ that the next number selected will be the _previously_ selected
 * number. As such, the periodicity is a fraction (or percentage if you like)
 * representing the chance of repetition.
 *
 * All other numbers in the range have an equal chance of being selected. The
 * chance of their selection is the remainder of the periodicity fraction
 * (subtracted from 1) shared equally amongst the other numbers. The remainder
 * allocation for each of the other numbers in the range can be represented as:
 *
 *\f[
     \frac{1 - periodicity}{rangeSize - 1}
 * \f]
 *
 * __Further Detail:__
 *
 * Having made an initial call to get a number, a subsequent call for a number
 * will be subject to the periodicity bias where the bias is in favour of the
 * previously selected number.
 *
 * Should this call (or any future call) result in the selection of a
 * _different_ number from the range, then that newly selected number will have
 * the periodicity bias applied to it upon a subsequent call to get a number.
 * And so on and so on.
 *
 * Upon instantiating the protocol, the initial number to be selected can be
 * provided to the constructor. This will be the number returned upon the first
 * call for a number and will have the periodicity bias applied to it for the
 * next call. If an initial number is not provided, the first call to get a
 * number will pick one from the range at random (equal probability / uniform
 * distribution).
 */
class Periodic : public Protocol {
  public:
    /*! @brief Takes a DiscreteGenerator derived from IDiscreteGenerator and a
     * Range
     *
     * @param generator Should be an instance of DiscreteGenerator. Default
     * construction is fine.
     *
     * @param range The range within which to produce numbers.
     *
     * @param chanceOfRepetition is a double between 0 and 1, representing a
     * fraction (or percentage) that determines the chance of the last selected
     * number being selected again upon another call to getNumber(). See
     * detailed description for more details.
     */
    Periodic(IDiscreteGenerator &generator,
             Range &range,
             double chanceOfRepetition);

    /*! @overload
     *
     * Accepts an additional parameter for setting the initial number to be
     * selected.
     *
     * @param initialSelection this number will be returned on the first call to
     * getNumber() and will have the periodicity bias applied to it for the next
     * call to getNumber()
     */
    Periodic(IDiscreteGenerator &generator,
             Range &range,
             double chanceOfRepetition,
             int initialSelection);

    ~Periodic();

    /*! @brief Returns a number according to the protocol.
     *
     * See the detailed description for more information.
     */
    int getNumber() override;

    /*! @brief Resets the state of the class to the state upon instantiation
     *
     * This means that if the class was instantiated with no initial number
     * selection, upon the next call to getNumber(), a number will be selected
     * at random with equal probability of selection from which to start the
     * periodicity process.
     *
     * If the class was instantiated with an initial number selection, this will
     * be the number returned from the next call to getNumber(), and will form
     * the starting point for the periodicity process from thereon.
     */
    void reset() override;

  private:
    Range &m_range;
    IDiscreteGenerator &m_generator;
    double m_periodicity;
    double m_remainderAllocation;
    void setPeriodicDistribution(int selectedIndex);
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
}}} // namespace actlib::Numbers::Steps

#endif /* Periodic_hpp */
