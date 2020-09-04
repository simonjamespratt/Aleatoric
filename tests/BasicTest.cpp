#include "Basic.hpp"

#include "Range.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>

SCENARIO("Numbers::Basic")
{
    GIVEN("Construction")
    {
        aleatoric::Range range(1, 3);

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(range.start, range.end));
                aleatoric::Basic(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 2;

        aleatoric::Range range(1, 3);

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));
        aleatoric::Basic instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number and returns it")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(generatedNumber == returnedNumber);
            }
        }

        WHEN("The range is changed")
        {
            THEN("The returned range should match the new range")
            {
                instance.setRange(aleatoric::Range(2, 10));
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            THEN("The generator should be set with the new range")
            {
                REQUIRE_CALL(*generatorPointer, setDistribution(3, 7));
                instance.setRange(aleatoric::Range(3, 7));
            }
        }
    }
}
