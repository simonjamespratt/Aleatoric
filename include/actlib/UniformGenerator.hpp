#ifndef UniformGenerator_hpp
#define UniformGenerator_hpp

#include "IUniformGenerator.hpp"

#include <pcg_random.hpp>
#include <random>

namespace actlib { namespace Numbers {
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
    @brief Constructor

    @param rangeStart start value for the uniform distribution
    @param rangeEnd end value for the uniform distribution
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
    pcg_extras::seed_seq_from<std::random_device> m_seedSource;
    pcg32 m_engine;
    std::uniform_int_distribution<int> m_distribution;
};
}} // namespace actlib::Numbers

#endif /* UniformGenerator_hpp */
