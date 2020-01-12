#ifndef Walk_hpp
#define Walk_hpp

#include "IUniformGenerator.hpp"
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
 *
 * Places a constraint on the production of numbers where only numbers within a
 * _sub-range around the last selected number_ can be selected. This forms a
 * variable stepwise traversal - or __walk__ - through the overall range.
 *
 * __Further detail__:
 *
 * Having made an initial call to get a number, a subsequent call for a number
 * will be limited to a _sub-range_ of the main range. The size of the sub-range
 * is limited by a _maximum step_ value so that the number selected must be no
 * further away (higher or lower) from the previously selected number than the
 * maximum step value. This can be expressed as:
 *
 * \f[
 *      previousNumber \pm maxStep
 * \f]
 *
 * The protocol will then select a number with equal probability from within the
 * sub-range.
 *
 * With each selection of a number, the sub-range is moved to surround the
 * number selected in readiness for the next call to get a number.
 *
 * Note that traversal of the range does _not_ wrap. In other words, if a
 * sub-range would otherwise encompass numbers outside the main range, the
 * sub-range is curtailed to avoid the selection of numbers outside the main
 * range. Here is an example:
 *
 * Given a main range of 1 to 10 and a maxStep of 5, if a number was selected
 * and it was 8, the natural calculation of the sub-range would be:
 *
 * \f[
 *      subRangeStart = 8 - 5 = 3 \\
 *      subRangeEnd = 8 + 5 = 13
 * \f]
 *
 * Clearly the subRangeEnd falls outside the main range (1, 10) and thus the
 * subRangeEnd would be adjusted to be 10.
 *
 * Upon instantiating the protocol, the initial number to be selected can be
 * provided to the constructor. This will be the starting point for the
 * traversal / walk of the range and also the number returned upon the first
 * call for a number. If an initial number is not provided, the first call to
 * get a number will pick one from the main range at random (equal probability /
 * uniform distribution).
 */
class Walk : public Protocol {
  public:
    /*!
     * @brief Construct a new Walk object
     *
     * @param generator An instance of UniformGenerator, derived from
     * IUniformGenerator. It should have been instantiated with an Engine, a
     * rangeStart and a rangeEnd. The rangeStart and rangeEnd should map to the
     * start and end of the range supplied respectively. An instance of the
     * generator initialised as above will provide an initial state of a uniform
     * distribution, and thus equal probability of selection, for each number
     * within the range.
     *
     * @param range The range supplied should map to the rangeStart and rangeEnd
     * supplied to the generator.
     *
     * @param maxStep The maximum step value used for calculating the sub-range
     * for the walk through the main range. For a detailed description of its
     * use in the protocol, see above. Note that the value supplied must not
     * exceed the size of the main range.
     */
    Walk(IUniformGenerator &generator, Range &range, int maxStep);

    /*!
     * @overload
     *
     * @param initialSelection This number will be returned upon the first call
     * to getNumber() and will be the starting point for the walk through the
     * range from thereon. Note that the number supplied must be within the
     * limits of the range supplied.
     */
    Walk(IUniformGenerator &generator,
         Range &range,
         int maxStep,
         int initialSelection);

    ~Walk();

    /*!
     * @return a number according to the protocol. See above.
     */
    int getNumber() override;

    /*!
     * @brief Resets the state of the class to the state upon instantiation
     *
     * This means that if the class was instantiated with no initial number
     * selection, upon the next call to getNumber(), a number will be selected
     * at random, with equal probability of selection, from which to start the
     * walk through the range.
     *
     * If the class was instantiated with an initial number selection, this will
     * be the number returned from the next call to getNumber(), and will form
     * the starting point for the walk through the range from thereon.
     */
    void reset() override;

  private:
    Range &m_range;
    IUniformGenerator &m_generator;
    int m_maxStep;
    void setForNextStep(int lastSelectedNumber);
    bool isOutOfRange(int number);
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
}} // namespace actlib::Numbers

#endif /* Walk_hpp */