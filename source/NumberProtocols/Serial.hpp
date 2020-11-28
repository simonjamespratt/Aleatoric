#ifndef Serial_hpp
#define Serial_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class SeriesPrinciple;
/*!
 * @brief A protocol for producing random numbers
 *
 * A concrete implementation of the Protocol interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 * (see Protocol for more information).
 *
 * This protocol follows the
 * [Serialism](https://en.wikipedia.org/wiki/Serialism) approach, allowing
 * the production of sets of numbers.
 *
 * __Further Detail__: An initial call to get a number will select a number
 * from within the range with equal probability. Subsequent calls to get a
 * number will prevent previously selected numbers from being selected again
 * until all other possible numbers in the range have been selected.
 */
class Serial : public NumberProtocol {
  public:
    Serial(std::unique_ptr<IDiscreteGenerator> generator);

    /*! @brief Takes a DiscreteGenerator derived from the
     * IDiscreteGenerator, and a Range.
     *
     * @param generator Should be an instance of DiscreteGenerator. Default
     * construction is fine.
     *
     * @param range The range within which to produce numbers.
     */
    Serial(std::unique_ptr<IDiscreteGenerator> generator, Range range);

    ~Serial();

    /*! @brief returns a random number according the the Serialism approach
     *
     * An initial call to get a number will select a number from within the
     * range with equal probability. Subsequent calls to get a number will
     * prevent previously selected numbers from being selected again until all
     * other possible numbers in the range have been selected.
     */
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    Range m_range;
    std::unique_ptr<SeriesPrinciple> m_seriesPrinciple;
};
} // namespace aleatoric

#endif /* Serial_hpp */
