#ifndef Walk_hpp
#define Walk_hpp

#include "IUniformGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
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
class Walk : public NumberProtocol {
  public:
    Walk(std::unique_ptr<IUniformGenerator> generator);

    /*!
     * @brief Construct a new Walk object
     *
     * @param generator An instance of UniformGenerator, derived from
     * IUniformGenerator. Default construction is fine.
     *
     * @param range The range within which to produce numbers.
     *
     * @param maxStep The maximum step value used for calculating the
     * sub-range for the walk through the main range. For a detailed
     * description of its use in the protocol, see above. Note that the
     * value supplied must not exceed the size of the main range.
     */
    Walk(std::unique_ptr<IUniformGenerator> generator,
         Range range,
         int maxStep);

    ~Walk();

    /*!
     * @return a number according to the protocol. See above.
     */
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolConfig newParams) override;

    NumberProtocolConfig getParams() override;

  private:
    std::unique_ptr<IUniformGenerator> m_generator;
    Range m_range;
    int m_maxStep;
    void setForNextStep(int lastSelectedNumber);
    bool m_haveRequestedFirstNumber;
    int m_lastNumberSelected;
    void checkMaxStepIsValid(int maxStep, Range range);
    void setRange(Range newRange);
};
} // namespace aleatoric

#endif /* Walk_hpp */
