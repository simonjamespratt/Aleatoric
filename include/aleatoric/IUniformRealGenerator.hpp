// Interface
#ifndef IUniformRealGenerator_hpp
#define IUniformRealGenerator_hpp

#include <utility>

namespace aleatoric {
class IUniformRealGenerator {
  public:
    virtual double getNumber() = 0;
    virtual void setDistribution(double rangeStart, double rangeEnd) = 0;
    virtual std::pair<double, double> getDistribution() = 0;
    virtual ~IUniformRealGenerator() = default;
};
} // namespace aleatoric

#endif /* IUniformRealGenerator_hpp */
