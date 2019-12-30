#ifndef DiscreteGenerator_hpp
#define DiscreteGenerator_hpp

#include "IDiscreteGenerator.hpp"
#include <random>
#include <vector>

namespace actlib { namespace Numbers {
/*! This is a comment */
class DiscreteGenerator : public IDiscreteGenerator {
  public:
    DiscreteGenerator(std::mt19937 &engine, std::vector<double> distribution);
    DiscreteGenerator(std::mt19937 &engine,
                      int vectorSize,
                      double uniformValue);
    ~DiscreteGenerator();
    int getNumber() override;
    void setDistributionVector(std::vector<double> distributionVector) override;
    void setDistributionVector(int vectorSize, double uniformValue) override;
    void updateDistributionVector(int index, double newValue) override;
    void updateDistributionVector(double uniformValue) override;
    std::vector<double> getDistributionVector() override;

  private:
    std::mt19937 _engine;
    std::vector<double> _distributionVector;
    std::discrete_distribution<int> _distribution;
    void setDistribution();
};
}} // namespace actlib::Numbers

#endif /* DiscreteGenerator_hpp */
