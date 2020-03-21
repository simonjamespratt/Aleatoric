#include "Basic.hpp"

#include "Range.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>

SCENARIO("Numbers::Basic")
{
    GIVEN("The class is instantiated")
    {
        WHEN("The object is constructed")
        {
            auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
            auto rangePointer = range.get();

            auto generator = std::make_unique<UniformGeneratorMock>();
            auto generatorPointer = generator.get();

            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(rangePointer->start, rangePointer->end));
                actlib::Numbers::Steps::Basic(std::move(generator),
                                              std::move(range));
            }
        }

        WHEN("A number is requested")
        {
            int generatedNumber = 2;

            auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
            auto rangePointer = range.get();

            auto generator = std::make_unique<UniformGeneratorMock>();
            auto generatorPointer = generator.get();

            ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));
            actlib::Numbers::Steps::Basic instance(std::move(generator),
                                                   std::move(range));

            THEN("It calls the generator to get a number and returns it")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(generatedNumber == returnedNumber);
            }
        }
    }
}
