#include "Walk.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Walk")
{
    GIVEN("The class is instantiated with an invalid max step")
    {
        WHEN("The value provided is greater than the range size")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidMaxStep = 11;

                REQUIRE_THROWS_AS(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 invalidMaxStep),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 invalidMaxStep),
                    "The value passed as argument for maxStep must be less "
                    "than or equal to 10");
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        int maxStep = 2;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<aleatoric::Range>(1, 10);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(rangePointer->start, rangePointer->end));
                aleatoric::Walk(std::move(generator), std::move(range), maxStep);
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        int maxStep = 2;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = std::make_unique<aleatoric::Range>(1, 10);
        auto rangePointer = range.get();

        aleatoric::Walk instance(std::move(generator), std::move(range), maxStep);

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
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(rangePointer->start, rangePointer->end));
                instance.reset();
            }
        }
    }

    GIVEN("The class is instantiated with an invalid initial number selection")
    {
        int maxStep = 1;

        WHEN("The value provided is greater than the range end")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                auto initialSelection = 11;

                REQUIRE_THROWS_AS(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 maxStep,
                                 initialSelection),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 maxStep,
                                 initialSelection),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 10");
            }
        }

        WHEN("The value provided is less than the range start")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                auto initialSelection = 0;

                REQUIRE_THROWS_AS(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 maxStep,
                                 initialSelection),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Walk(std::make_unique<aleatoric::UniformGenerator>(),
                                 std::make_unique<aleatoric::Range>(1, 10),
                                 maxStep,
                                 initialSelection),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 10");
            }
        }
    }

    GIVEN("The class is instantiated with a valid initial number selection")
    {
        int maxStep = 2;
        int initialSelection = 4;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = std::make_unique<aleatoric::Range>(1, 10);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range start and end")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(rangePointer->start, rangePointer->end));

                aleatoric::Walk(std::move(generator),
                             std::move(range),
                             maxStep,
                             initialSelection);
            }
        }
    }

    GIVEN("The class is instantiated with a valid initial number selection")
    {
        int maxStep = 2;
        int initialSelection = 4;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = std::make_unique<aleatoric::Range>(1, 10);
        auto rangePointer = range.get();

        aleatoric::Walk instance(std::move(generator),
                              std::move(range),
                              maxStep,
                              initialSelection);

        WHEN("The first number is requested")
        {
            THEN("It should not call the generator for a number but return the "
                 "initial selection")
            {
                FORBID_CALL(*generatorPointer, getNumber());
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            AND_THEN("Sets the the generator distribution in readiness for "
                     "next call to get a number")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(ANY(int), ANY(int)));
                instance.getIntegerNumber();
            }
        }

        WHEN("A subsequent number is requested")
        {
            instance.getIntegerNumber(); // first call

            THEN("It does call the generator for a number and return it")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).TIMES(1).RETURN(1);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == 1);
            }

            AND_THEN("Sets the the generator distribution in readiness for "
                     "next call to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).TIMES(1).RETURN(1);
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(ANY(int), ANY(int)));
                instance.getIntegerNumber();
            }
        }

        WHEN("A reset is performed following a previous call to get a number")
        {
            instance.getIntegerNumber(); // unsets the initial state

            AND_WHEN("The next number is requested")
            {
                THEN("The initial selection is returned, due to the initial "
                     "state having been reinstated during the reset")
                {
                    instance.reset();
                    FORBID_CALL(*generatorPointer, getNumber());
                    auto returnedNumber = instance.getIntegerNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
