#ifndef UniformRealGenerator_hpp
#define UniformRealGenerator_hpp

#include <memory>
#include <random>

namespace aleatoric {
class Engine;
class UniformRealGenerator {
  public:
    UniformRealGenerator();
    UniformRealGenerator(double rangeStart, double rangeEnd);
    ~UniformRealGenerator();

    double getNumber();
    void setDistribution(double rangeStart, double rangeEnd);
    std::pair<double, double> getDistribution();

  private:
    std::unique_ptr<Engine> m_engine;
    std::uniform_real_distribution<double> m_distribution;
    std::pair<double, double> m_range;
};
} // namespace aleatoric

#endif /* UniformRealGenerator_hpp */
