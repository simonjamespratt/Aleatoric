#include "Basic.hpp"

#include "Range.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Basic")
{
    GIVEN("The class is instantiated")
    {
        actlib::Numbers::Range range(1, 3);
        UniformGeneratorMock generator;
        ALLOW_CALL(generator, setDistribution(ANY(int), ANY(int)));
        actlib::Numbers::Steps::Basic instance(generator, range);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(generator,
                             setDistribution(range.start, range.end));
                actlib::Numbers::Steps::Basic(generator, range);
            }
        }

        WHEN("A number is requested")
        {
            int generatedNumber = 2;

            THEN("It calls the generator to get a number and returns it")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(generatedNumber == returnedNumber);
            }
        }
    }
}
