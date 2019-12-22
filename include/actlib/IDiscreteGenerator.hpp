// Interface
// TODO: This is lazy naming. Consider how this interface and its
// abstraction/implementation naming could be better

#ifndef IDiscreteGenerator_hpp
#define IDiscreteGenerator_hpp

#include <vector>

namespace actlib { namespace Numbers {
class IDiscreteGenerator {
  public:
    virtual int getNumber() = 0;
    virtual void
    setDistributionVector(std::vector<double> distributionVector) = 0;
    virtual void setDistributionVector(int vectorSize, double uniformValue) = 0;
    virtual void updateDistributionVector(int index, double newValue) = 0;
    virtual void updateDistributionVector(double uniformValue) = 0;
    virtual std::vector<double> getDistributionVector() = 0;
    virtual ~IDiscreteGenerator() = default;
};
}} // namespace actlib::Numbers

#endif /* IDiscreteGenerator_hpp */
