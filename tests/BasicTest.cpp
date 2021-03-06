#include "Basic.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>

SCENARIO("Numbers::Basic: default constructor")
{
    using namespace aleatoric;

    Basic instance(std::make_unique<UniformGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
    }

    THEN("A set of numbers should be as expected")
    {
        for(int i = 0; i < 1000; i++) {
            auto number = instance.getIntegerNumber();
            REQUIRE((number == 0 || number == 1));
        }
    }
}

SCENARIO("Numbers::Basic")
{
    using namespace aleatoric;

    GIVEN("Construction")
    {
        Range range(1, 3);

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(range.start, range.end));
                Basic(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 2;

        Range range(1, 3);

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));
        Basic instance(std::move(generator), range);

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
    }
}

SCENARIO("Numbers::Basic: params")
{
    using namespace aleatoric;

    Basic instance(std::make_unique<UniformGenerator>(), Range(1, 10));

    WHEN("Get params")
    {
        THEN("should match the object state")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 10);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocol::Type::basic);
        }
    }

    WHEN("Set params")
    {
        Range newRange(20, 30);
        NumberProtocolConfig newParams(newRange,
                                       NumberProtocolParams(BasicParams()));
        instance.setParams(newParams);

        THEN("object state is updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }

        AND_WHEN("A set of numbers is requested")
        {
            std::vector<int> set(1000);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("All values will be from new range")
            {
                for(auto &&i : set) {
                    REQUIRE(newRange.numberIsInRange(i));
                }
            }
        }
    }
}
