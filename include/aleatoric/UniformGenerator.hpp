#ifndef UniformGenerator_hpp
#define UniformGenerator_hpp

#include "IUniformGenerator.hpp"

#include <memory>
#include <random>

namespace aleatoric {
class Engine;
/*!
@brief Implementation class for generating numbers from a uniform
distribution

Uses a [Permuted Congruential Generator -
PCG](https://github.com/imneme/pcg-cpp) engine through which to produce random
numbers according to a uniform distribution.

The uniform distribution is realised with __std::uniform_int_distribution__
*/
class UniformGenerator : public IUniformGenerator {
  public:
    /*!
     * @brief Constructor
     *
     * Creates a generator with a default distribution that has a range of 0 to
     * 1 (heads or tails). Calling getNumber() will output 0 or 1 with equal
     * probability of selection.
     *
     * The range of the distribution can be changed at any time by calling
     * setDistribution().
     *
     */
    UniformGenerator();

    /*!
     * @brief Constructor
     *
     * Creates a generator with a distribution as defined by the rangeStart and
     * rangeEnd provided (inclusive / closed range). Each integer within the
     * range has an equal probability of selection when calling getNumber().
     *
     * @param rangeStart start value for the uniform distribution
     * @param rangeEnd end value for the uniform distribution
     */
    UniformGenerator(int rangeStart, int rangeEnd);

    ~UniformGenerator();

    /*! @brief returns random numbers filtered through the uniform distribution
     */
    int getNumber() override;

    /*!
    @brief sets the range of the uniform distribution. The range is inclusive.

    @param rangeStart new value for the start of the distribution
    @param rangeEnd new value for the end of the distribution
    */
    void setDistribution(int rangeStart, int rangeEnd) override;

  private:
    std::unique_ptr<Engine> m_engine;
    std::uniform_int_distribution<int> m_distribution;
};
} // namespace aleatoric

#endif /* UniformGenerator_hpp */
