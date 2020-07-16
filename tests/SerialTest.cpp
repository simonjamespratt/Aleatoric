#include "Serial.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Serial")
{
    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        aleatoric::Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                aleatoric::Serial(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(generatedNumber);
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(generatedNumber, 0.0));

        // this is for reset()
        ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));

        // ensures that seriesIsComplete returns true
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        aleatoric::Range range(1, 3);

        aleatoric::Serial instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                instance.getIntegerNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getIntegerNumber();
            }

            THEN("It returns the selected number, offset by the range.offset")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto number = instance.getIntegerNumber();
                REQUIRE(number == generatedNumber + range.offset);
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

        AND_WHEN("A range change is made")
        {
            // NB: using real dependencies
            aleatoric::Serial realInstance(
                std::make_unique<aleatoric::DiscreteGenerator>(),
                aleatoric::Range(1, 3));

            aleatoric::Range newRange(4, 9); // size 6

            std::vector<int> expectedValues {4, 5, 6, 7, 8, 9};
            auto expectedValuesPermutation =
                Catch::UnorderedEquals(expectedValues);

            std::vector<int> set(newRange.size);

            THEN("The returned range should match the new range")
            {
                realInstance.setRange(newRange);
                auto returnedRange = realInstance.getRange();
                REQUIRE(returnedRange.start == newRange.start);
                REQUIRE(returnedRange.end == newRange.end);
            }

            AND_WHEN("change is made before a number has been returned")
            {
                realInstance.setRange(newRange);

                for(auto &&i : set) {
                    i = realInstance.getIntegerNumber();
                }

                THEN("A set covering the range will have the expected result")
                {
                    REQUIRE_THAT(set, expectedValuesPermutation);
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

                THEN("A set covering the range will have the expected result")
                {
                    REQUIRE_THAT(set, expectedValuesPermutation);
                }
            }
        }
    }
}
