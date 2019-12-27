#include "Basic.hpp"
#include "IUniformGenerator.hpp"
#include "Range.hpp"
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class UniformGeneratorMock : public actlib::Numbers::IUniformGenerator {
  public:
    MAKE_MOCK0(getNumber, int());
    MAKE_MOCK2(setDistribution, void(int, int));
};

SCENARIO("Numbers::Basic") {
    GIVEN("The class is instantiated correctly") {
        actlib::Numbers::Range range(1, 3);
        UniformGeneratorMock generator;
        actlib::Numbers::Basic instance(generator, range);
        WHEN("A number is requested") {
            int generatedNumber = 2;

            THEN("It calls the generator to get a number and returns it") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(generatedNumber == returnedNumber);
            }
        }
    }
}
