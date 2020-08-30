#include "Walk.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Walk: default constructor")
{
    using namespace aleatoric;

    Walk instance(std::make_unique<UniformGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto maxStep = params.protocols.getWalk().getMaxStep();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(maxStep == 1);
    }

    std::vector<int> set(1000);
    for(auto &&i : set) {
        i = instance.getIntegerNumber();
    }

    THEN("A set is within range")
    {
        for(auto &&i : set) {
            REQUIRE((i == 0 || i == 1));
        }
    }

    THEN("Each item in a set will be within max step of the last")
    {
        std::vector<int> differences(set.size());
        std::adjacent_difference(set.begin(), set.end(), differences.begin());

        //  remove the first element as it is the value of the first
        //  element in the set (and not a difference between two
        //  numbers in the set)
        differences.erase(differences.begin());

        for(auto &&i : differences) {
            REQUIRE(i <= 1);
        }
    }
}

SCENARIO("Numbers::Walk")
{
    using namespace aleatoric;

    GIVEN("Construction: with an invalid max step")
    {
        WHEN("The value provided is greater than the range size")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidMaxStep = 11;

                REQUIRE_THROWS_AS(Walk(std::make_unique<UniformGenerator>(),
                                       Range(1, 10),
                                       invalidMaxStep),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Walk(std::make_unique<UniformGenerator>(),
                         Range(1, 10),
                         invalidMaxStep),
                    "The value passed as argument for maxStep must be less "
                    "than or equal to 10");
            }
        }
    }

    GIVEN("Construction")
    {
        int maxStep = 2;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        Range range(1, 10);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(range.start, range.end));
                Walk(std::move(generator), range, maxStep);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int maxStep = 2;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        Range range(1, 10);

        Walk instance(std::move(generator), range, maxStep);

        WHEN("A number is requested")
        {
            THEN("It returns a generated number")
            {
                // no need for adding of offset as UniformDistribution takes the
                // range.start and range.end as its range
                REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(1);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == 1);
            }

            AND_WHEN("That generated number is mid range")
            {
                // mid-range here meaning that the new distribution range
                // won't exceed the main range
                THEN("It sets the generator distribution to the correct range "
                     "in readiness for next call to get a number")
                {
                    // The range should be the maxStep in both directions with
                    // the last selected number in the middle. Range is
                    // inclusive. i.e:

                    // rangeStart = lastSelected - maxStep,
                    // rangeEnd = lastSelected + maxStep

                    REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(4);
                    REQUIRE_CALL(*generatorPointer, setDistribution(2, 6));
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("That generated number is start of range")
            {
                // start-of-range here meaning that the new distribution range
                // set will exceed the main range start
                THEN("It sets the generator distribution to the correct range "
                     "in readiness for next call to get a number")
                {
                    // The logic does not account for wrapping and maxStep range
                    // is curtailed if it hits either end of the main range
                    REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(2);
                    REQUIRE_CALL(*generatorPointer,
                                 setDistribution(1, 4)); // instead of (0, 4)
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("That generated number is end of range")
            {
                // end-of-range here meaning that the new distribution range
                // set will exceed the main range end
                THEN("It sets the generator distribution to the correct range "
                     "in readiness for next call to get a number")
                {
                    // The logic does not account for wrapping and maxStep range
                    // is curtailed if it hits either end of the main range
                    REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(9);
                    REQUIRE_CALL(*generatorPointer,
                                 setDistribution(7, 10)); // instead of (7, 11)
                    instance.getIntegerNumber();
                }
            }
        }

        WHEN("A reset is performed")
        {
            THEN("The generator distribution is set to the full range")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(range.start, range.end));
                instance.reset();
            }
        }
    }
}

SCENARIO("Numbers::Walk: params")
{
    using namespace aleatoric;

    int maxStep = 5;
    Walk instance(std::make_unique<UniformGenerator>(), Range(1, 10), maxStep);

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("maxStep state is returned")
        {
            REQUIRE(params.protocols.getWalk().getMaxStep() == maxStep);
        }

        THEN("range state is returned")
        {
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 10);
        }

        THEN("active protocol is set correctly")
        {
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocolParameters::Protocols::ActiveProtocol::walk);
        }
    }

    WHEN("Set params")
    {
        int newMaxStep = 3;
        Range newRange(20, 30);

        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Walk(newMaxStep)));

        instance.setParams(newParams);
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("maxStep is updated")
        {
            REQUIRE(params.protocols.getWalk().getMaxStep() == 3);
        }

        THEN("range is updated")
        {
            REQUIRE(returnedRange.start == 20);
            REQUIRE(returnedRange.end == 30);
        }

        AND_WHEN("A set of numbers is gathered")
        {
            std::vector<int> set(1000);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("All numbers should be in the new range")
            {
                for(auto &&i : set) {
                    REQUIRE(newRange.numberIsInRange(i));
                }
            }

            THEN("All numbers in the set should be no more than the "
                 "new maxStep from the last number")
            {
                std::vector<int> differences(set.size());
                std::adjacent_difference(set.begin(),
                                         set.end(),
                                         differences.begin());

                //  remove the first element as it is the value of the first
                //  element in the set (and not a difference between two
                //  numbers in the set)
                differences.erase(differences.begin());

                for(auto &&i : differences) {
                    REQUIRE(i <= newMaxStep);
                }
            }
        }
    }

    WHEN("Set params: The last returned number is within new range")
    {
        auto lastNumber = instance.getIntegerNumber();

        int newMaxStep = 3;
        Range newRange(lastNumber - 20, lastNumber + 20);

        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Walk(newMaxStep)));

        instance.setParams(newParams);

        THEN("The next number should be no more than the new maxStep from the "
             "last number")
        {
            auto nextNumber = instance.getIntegerNumber();
            auto difference = std::abs(lastNumber - nextNumber);
            REQUIRE(difference <= newMaxStep);
        }
    }

    WHEN("Set params: check maxStep value is valid")
    {
        Range newRange(20, 30);

        AND_WHEN("it is greater than new range size")
        {
            auto newMaxStep = newRange.size + 1;
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Walk(newMaxStep)));

            THEN("An exception is thrown")
            {
                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }

        AND_WHEN("it is less than 1")
        {
            int newMaxStep = 0;
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Walk(newMaxStep)));

            THEN("An exception is thrown")
            {
                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }
    }
}
