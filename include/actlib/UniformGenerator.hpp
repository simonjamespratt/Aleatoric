#ifndef UniformGenerator_hpp
#define UniformGenerator_hpp

#include "IUniformGenerator.hpp"
#include <random>

namespace actlib { namespace Numbers {
class UniformGenerator : public IUniformGenerator {
  public:
    UniformGenerator(std::mt19937 &engine, int rangeStart, int rangeEnd);
    ~UniformGenerator();
    int getNumber() override;
    void setDistribution(int rangeStart, int rangeEnd) override;

  private:
    std::mt19937 _engine;
    std::uniform_int_distribution<int> _distribution;
};
}} // namespace actlib::Numbers

#endif /* UniformGenerator_hpp */
