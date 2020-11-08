#ifndef UniformRealGenerator_hpp
#define UniformRealGenerator_hpp

#include "IUniformRealGenerator.hpp"

#include <pcg_random.hpp>
#include <random>

namespace aleatoric {
class UniformRealGenerator : public IUniformRealGenerator {
  public:
    UniformRealGenerator();
    UniformRealGenerator(double rangeStart, double rangeEnd);
    ~UniformRealGenerator();

    double getNumber() override;
    void setDistribution(double rangeStart, double rangeEnd) override;
    std::pair<double, double> getDistribution() override;

  private:
    pcg_extras::seed_seq_from<std::random_device> m_seedSource;
    pcg32 m_engine;
    std::uniform_real_distribution<double> m_distribution;
    std::pair<double, double> m_range;
};
} // namespace aleatoric

#endif /* UniformRealGenerator_hpp */
