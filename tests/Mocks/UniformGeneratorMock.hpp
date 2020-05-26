#ifndef UniformGeneratorMock_hpp
#define UniformGeneratorMock_hpp

#include "IUniformGenerator.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class UniformGeneratorMock : public aleatoric::IUniformGenerator {
  public:
    MAKE_MOCK0(getNumber, int(), override);
    MAKE_MOCK2(setDistribution, void(int, int), override);
};

#endif /* UniformGeneratorMock_hpp */
