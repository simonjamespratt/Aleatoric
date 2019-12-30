#ifndef UniformGenerator_hpp
#define UniformGenerator_hpp

#include "IUniformGenerator.hpp"
#include <random>

namespace actlib { namespace Numbers {
/*!
@brief Implementation class for generating numbers from a uniform
distribution

Uses a Mersenne Twister 19937 generator as an engine through which to
produce random numbers according to a uniform distribution.

The uniform distribution is realised with __std::uniform_int_distribution__
*/
class UniformGenerator : public IUniformGenerator {
  public:
    /*!
    @brief Constructor

    @param engine a Mersenne Twister 19937 generator of type **std::mt19937**
    @param rangeStart start value for the uniform distribution
    @param rangeEnd end value for the uniform distribution
    */
    UniformGenerator(std::mt19937 &engine, int rangeStart, int rangeEnd);
    ~UniformGenerator();
    /*! @brief returns random numbers filtered through the uniform distribution
     */
    int getNumber() override;
    /*!
    @brief sets the range of the uniform distribution

    @param rangeStart new value for the start of the distribution
    @param rangeEnd new value for the end of the distribution
    */
    void setDistribution(int rangeStart, int rangeEnd) override;

  private:
    std::mt19937 _engine;
    std::uniform_int_distribution<int> _distribution;
};
}} // namespace actlib::Numbers

#endif /* UniformGenerator_hpp */
