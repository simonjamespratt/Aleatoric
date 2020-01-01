#ifndef DiscreteGenerator_hpp
#define DiscreteGenerator_hpp

#include "IDiscreteGenerator.hpp"
#include <random>

namespace actlib { namespace Numbers {
/*!
@brief Implementation class for generating numbers from a discrete distribution

Uses a Mersenne Twister 19937 generator as an engine through which to produce
random numbers according to a discrete distribution.

The discrete distribution is realised with  __std::discrete_distribution__
*/
class DiscreteGenerator : public IDiscreteGenerator {
  public:
    /*!
    @brief Constructor for instantiating an instance of the class by
     * providing a fully customised distribution vector
     *
     * @param engine a Mersenne Twister 19937 generator of type **std::mt19937**
     * @param distribution fully formed vector representing the discrete
    distribution to be used by the class
     */
    DiscreteGenerator(std::mt19937 &engine, std::vector<double> distribution);

    /*! @brief Constructor for instantiating an instance of the class by
     * specifying a distribution size and uniform value
     *
     * Creates a distribution vector by setting the entire vector to have equal
     * values and therefore produces a uniform distribution
     *
     * @param engine a Mersenne Twister 19937 generator of type **std::mt19937**
     * @param vectorSize the size of the distribution vector to create
     * @param uniformValue the value to set for each item in the vector
     */
    DiscreteGenerator(std::mt19937 &engine,
                      int vectorSize,
                      double uniformValue);

    ~DiscreteGenerator();

    /*! @brief returns generated numbers according to the discrete dsitribution
     * created */
    int getNumber() override;

    /*! @brief sets the entire vector for the
     * distribution to a fully customised distribution
     *
     * @param distributionVector customised distribution vector
     */
    void setDistributionVector(std::vector<double> distributionVector) override;

    /*! @brief sets the entire vector to have equal
     * values and therefore produce a uniform distribution
     *
     * @param vectorSize size of the distribution vector to create
     * @param uniformValue the value to set for each item in the vector
     */
    void setDistributionVector(int vectorSize, double uniformValue) override;

    /*! @brief updates an item in the distribution vector, identified by index,
     * to a new value
     *
     * @param index index of the item in the vector to update
     * @param newValue value to set for the identified vector item
     */
    void updateDistributionVector(int index, double newValue) override;

    /*! @brief updates the entire distribution vector
     * to have equal values and therefore produce a uniform distribution
     *
     * @param uniformValue the value which is set for each item in the
     * distribution vector */
    void updateDistributionVector(double uniformValue) override;

    /*! @brief returns the current state of the distribution vector */
    std::vector<double> getDistributionVector() override;

  private:
    std::mt19937 _engine;
    std::vector<double> _distributionVector;
    std::discrete_distribution<int> _distribution;
    void setDistribution();
};
}} // namespace actlib::Numbers

#endif /* DiscreteGenerator_hpp */