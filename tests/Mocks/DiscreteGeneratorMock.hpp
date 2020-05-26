#ifndef DiscreteGeneratorMock_hpp
#define DiscreteGeneratorMock_hpp

#include "IDiscreteGenerator.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class DiscreteGeneratorMock : public aleatoric::IDiscreteGenerator {
  public:
    MAKE_MOCK0(getNumber, int(), override);
    MAKE_MOCK1(setDistributionVector, void(std::vector<double>), override);
    MAKE_MOCK2(setDistributionVector, void(int, double), override);
    MAKE_MOCK2(updateDistributionVector, void(int, double), override);
    MAKE_MOCK1(updateDistributionVector, void(double), override);
    MAKE_MOCK0(getDistributionVector, std::vector<double>(), override);
};

#endif /* DiscreteGeneratorMock_hpp */
