// Interface
// TODO: RE-NAME: This is lazy naming. Consider how this interface and its
// abstraction/implementation naming could be better

#ifndef IDiscreteGenerator_hpp
#define IDiscreteGenerator_hpp

#include <vector>

namespace aleatoric {
/*! @brief An interface abstract class from which the DiscreteGenerator
 * class is derived */
class IDiscreteGenerator {
  public:
    /*! @brief pure virtual method for returning generated numbers */
    virtual int getNumber() = 0;

    /*! @brief pure virtual method for setting the distribution vector */
    virtual void
    setDistributionVector(std::vector<double> distributionVector) = 0;

    /*! @brief pure virtual method for setting the distribution vector */
    virtual void setDistributionVector(int vectorSize, double uniformValue) = 0;

    /*! @brief pure virtual method for updating an item in the distirbution
     * vector */
    virtual void updateDistributionVector(int index, double newValue) = 0;

    /*! @brief pure virtual method for updating the distribution vector */
    virtual void updateDistributionVector(double uniformValue) = 0;

    /*! @brief pure virtual method for getting the distribution vector */
    virtual std::vector<double> getDistributionVector() = 0;
    virtual ~IDiscreteGenerator() = default;
};
} // namespace aleatoric

#endif /* IDiscreteGenerator_hpp */
