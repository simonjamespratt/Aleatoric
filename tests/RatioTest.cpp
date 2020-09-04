#include "Ratio.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Ratio")
{
    GIVEN("Construction: with mismatched range and ratios collection")
    {
        WHEN("The constructor is called")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                // should have a size of 3 if it were to match the range size
                std::vector<int> ratios {1, 2};

                REQUIRE_THROWS_AS(
                    aleatoric::Ratio(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(10, 12),
                        ratios),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Ratio(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(10, 12),
                        ratios),
                    "The size of ratios collection must match the size of the "
                    "range");
            }
        }
    }

    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        // 3 numbers because the range is inclusive
        std::vector<int> ratios {1, 2, 3};
        int ratiosSum = 6;

        WHEN("The constructor is called")
        {
            THEN("The generator should be set to the size of the sum of the "
                 "ratios supplied with equal probability")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(ratiosSum, 1.0));
                aleatoric::Ratio(std::move(generator),
                                 aleatoric::Range(10, 12),
                                 ratios);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(1);
        ALLOW_CALL(*generatorPointer,
                   setDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(ANY(int), ANY(double)));
        // ensures that seriesIsComplete returns true
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        aleatoric::Range range(10, 12);

        // 3 numbers because the range is inclusive
        std::vector<int> ratios {1, 3, 5};

        aleatoric::Ratio instance(std::move(generator), range, ratios);

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(1);
                instance.getIntegerNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                int generatedNumber = 1;

                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);

                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));

                instance.getIntegerNumber();
            }

            THEN("It returns the number held within the selectables collection "
                 "at the index specified by the generated number")
            {
                std::vector<int>
                    expectedSelectables {10, 11, 11, 11, 12, 12, 12, 12, 12};

                // get generator to return numbers for each index in
                // expectedSelectables
                for(int i = 0; i < expectedSelectables.size(); i++) {
                    REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(i);

                    auto returnedNumber = instance.getIntegerNumber();

                    REQUIRE(returnedNumber == expectedSelectables[i]);
                }
            }

            THEN("It gets the state of the generator distribution for "
                 "determining if the series is complete")
            {
                // NB: the return value for this test can be anything
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getIntegerNumber();
            }

            AND_WHEN("The series is complete")
            {
                THEN("It resets the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {0.0});

                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(1.0));

                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The series is not complete")
            {
                THEN("It does not reset the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {1.0});

                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(1.0));

                    instance.getIntegerNumber();
                }
            }
        }

        WHEN("A reset is requested")
        {
            THEN("It resets the generator distribution")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }

        WHEN("The ranged is changed")
        {
            // NB: using real dependencies

            aleatoric::Range range(10, 12);
            std::vector<int> ratios {1, 2, 3};
            // expected selectables: 10, 11, 11, 12, 12, 12

            aleatoric::Ratio realInstance(
                std::make_unique<aleatoric::DiscreteGenerator>(),
                range,
                ratios);

            THEN("The returned range should match the new range")
            {
                aleatoric::Range newRange(1, 8);
                realInstance.setRange(newRange);
                auto returnedRange = realInstance.getRange();

                REQUIRE(returnedRange.start == newRange.start);
                REQUIRE(returnedRange.end == newRange.end);
            }

            AND_WHEN("New range is same size as old range")
            {
                aleatoric::Range newRange(6, 8);

                std::vector<int> expectedSelectables {6, 7, 7, 8, 8, 8};
                auto expectedSelectablesPermutation =
                    Catch::UnorderedEquals(expectedSelectables);

                std::vector<int> set(expectedSelectables.size());

                AND_WHEN("change is made before a number has been returned")
                {
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }

                AND_WHEN("change is made after some numbers have been returned")
                {
                    // request some numbers
                    for(int i = 0; i < 3; i++) {
                        realInstance.getIntegerNumber();
                    }

                    // then change the range
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }
            }

            AND_WHEN("New range size is larger than the old range")
            {
                // TODO: DYNAMIC-PARAMS: this is an interim measure. In future,
                // we'll have to check that an update to params has a matching
                // range size and ratios size

                aleatoric::Range newRange(4, 8); // size of 5

                // for now we'll add values of 1 to the end of the ratios
                // collection as "defaults" for the difference in size, so:

                // ratios: {1, 2, 3, 1, 1}

                std::vector<int> expectedSelectables {4, 5, 5, 6, 6, 6, 7, 8};
                auto expectedSelectablesPermutation =
                    Catch::UnorderedEquals(expectedSelectables);

                std::vector<int> set(expectedSelectables.size());

                AND_WHEN("change is made before a number has been returned")
                {
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }

                AND_WHEN("change is made after some numbers have been returned")
                {
                    // request some numbers
                    for(int i = 0; i < 3; i++) {
                        realInstance.getIntegerNumber();
                    }

                    // then change the range
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }
            }

            AND_WHEN("New range size is smaller than the old range")
            {
                // TODO: DYNAMIC-PARAMS: this is an interim measure. In future,
                // we'll have to check that an update to params has a matching
                // range size and ratios size

                aleatoric::Range newRange(4, 5); // size of 2

                // for now we'll remove values from the end of the
                // ratios collection to counter the difference in size, so:

                // ratios: {1, 2}

                std::vector<int> expectedSelectables {4, 5, 5};
                auto expectedSelectablesPermutation =
                    Catch::UnorderedEquals(expectedSelectables);

                std::vector<int> set(expectedSelectables.size());

                AND_WHEN("change is made before a number has been returned")
                {
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }

                AND_WHEN("change is made after some numbers have been returned")
                {
                    // request some numbers
                    for(int i = 0; i < 3; i++) {
                        realInstance.getIntegerNumber();
                    }

                    // then change the range
                    realInstance.setRange(newRange);
                    for(auto &&i : set) {
                        i = realInstance.getIntegerNumber();
                    }

                    THEN("A set covering all selectables will have the "
                         "expected values")
                    {
                        REQUIRE_THAT(set, expectedSelectablesPermutation);
                    }
                }
            }
        }
    }
}
