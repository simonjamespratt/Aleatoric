#ifndef NoRepetition_hpp
#define NoRepetition_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {

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
class NoRepetition : public NumberProtocol {
  public:
    /*! @brief Takes a DiscreteGenerator derived from the IDiscreteGenerator,
     * and a Range
     *
     * @param generator should be an instance of DiscreteGenerator. Default
     * construction is fine.
     *
     * @param range The range within which to produce numbers.
     */
    NoRepetition(std::unique_ptr<IDiscreteGenerator> generator,
                 std::unique_ptr<Range> range);

    ~NoRepetition();

    /*! @brief returns a random number preventing immediate repetition of the
     * last selected number
     *
     * An initial call to get a number will select a number from within
     * the range with equal probability. The next call to get a number will
     * prevent this number from being selected, whilst all other numbers in the
     * range have an equal probability of being selected.
     */
    int getIntegerNumber() override;
    double getDecimalNumber() override;

    /*! @brief will reset the distibution to the initial state of a uniform
     * distribution
     *
     * Where each number in the range has equal probability of being selected
     * upon the next call to @ref getNumber */
    void reset() override;

  private:
    std::unique_ptr<Range> m_range;
    std::unique_ptr<IDiscreteGenerator> m_generator;
};
} // namespace aleatoric

#endif /* NoRepetition_hpp */
