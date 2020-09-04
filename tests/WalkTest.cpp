#include "Walk.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Walk")
{
    GIVEN("Construction: with an invalid max step")
    {
        WHEN("The value provided is greater than the range size")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidMaxStep = 11;

                REQUIRE_THROWS_AS(
                    aleatoric::Walk(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        aleatoric::Range(1, 10),
                        invalidMaxStep),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Walk(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        aleatoric::Range(1, 10),
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

        aleatoric::Range range(1, 10);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(range.start, range.end));
                aleatoric::Walk(std::move(generator), range, maxStep);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int maxStep = 2;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        aleatoric::Range range(1, 10);

        aleatoric::Walk instance(std::move(generator), range, maxStep);

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

    GIVEN("The object is constructed with real instance")
    {
        int maxStep = 5;
        aleatoric::Walk realInstance(
            std::make_unique<aleatoric::UniformGenerator>(),
            aleatoric::Range(1, 10),
            maxStep);

        WHEN("The range is changed")
        {
            THEN("The returned range should match the one received")
            {
                aleatoric::Range newRange(11, 30);
                realInstance.setRange(newRange);
                auto returnedRange = realInstance.getRange();
                REQUIRE(returnedRange.start == newRange.start);
                REQUIRE(returnedRange.end == newRange.end);
            }

            AND_WHEN("A set of numbers is gathered")
            {
                aleatoric::Range newRange(11, 30);
                realInstance.setRange(newRange);
                std::vector<int> set(100);
                for(auto &&i : set) {
                    i = realInstance.getIntegerNumber();
                }

                THEN("All numbers in the set should be within the new range")
                {
                    for(auto &&i : set) {
                        REQUIRE(i >= newRange.start);
                        REQUIRE(i <= newRange.end);
                    }
                }

                THEN("All numbers in the set should be no more than the "
                     "maxStep from the last number")
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
                        REQUIRE(i <= maxStep);
                    }
                }
            }

            AND_WHEN("The number last selected before the range change is "
                     "within the new range")
            {
                // old range: 1, 10
                auto lastNumber = realInstance.getIntegerNumber();
                aleatoric::Range newRange(lastNumber - 100, lastNumber + 100);
                realInstance.setRange(newRange);

                THEN("The next selected number should be no more than the "
                     "maxStep from the number last selected before the range "
                     "change")
                {
                    auto nextNumber = realInstance.getIntegerNumber();
                    auto difference = std::abs(lastNumber - nextNumber);
                    REQUIRE(difference <= maxStep);
                }
            }
        }
    }
}
