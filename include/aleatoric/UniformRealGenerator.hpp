#ifndef UniformRealGenerator_hpp
#define UniformRealGenerator_hpp

#include <pcg_random.hpp>
#include <random>

namespace aleatoric {
class UniformRealGenerator {
  public:
    UniformRealGenerator();
    UniformRealGenerator(double rangeStart, double rangeEnd);
    ~UniformRealGenerator();

    double getNumber();
    void setDistribution(double rangeStart, double rangeEnd);
    std::pair<double, double> getDistribution();

  private:
    pcg_extras::seed_seq_from<std::random_device> m_seedSource;
    pcg32 m_engine;
    std::uniform_real_distribution<double> m_distribution;
    std::pair<double, double> m_range;
};
} // namespace aleatoric

#endif /* UniformRealGenerator_hpp */
