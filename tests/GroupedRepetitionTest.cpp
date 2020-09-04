#include "GroupedRepetition.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::GroupedRepetition")
{
    GIVEN("Construction")
    {
        auto numberGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto numberGeneratorPointer = numberGenerator.get();
        ALLOW_CALL(*numberGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));

        auto groupingGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto groupingGeneratorPointer = groupingGenerator.get();
        ALLOW_CALL(*groupingGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));

        aleatoric::Range range(11, 13);
        std::vector<int> groupings {1, 2, 3};

        WHEN("In the constructor")
        {
            THEN("The number generator should be set to the size of the range "
                 "with an equal probability setting")
            {
                REQUIRE_CALL(*numberGeneratorPointer,
                             setDistributionVector(range.size, 1.0));
                aleatoric::GroupedRepetition(std::move(numberGenerator),
                                             std::move(groupingGenerator),
                                             range,
                                             groupings);
            }

            THEN("The grouping generator should be set to the size of the "
                 "groupings collection with an equal probability setting")
            {
                REQUIRE_CALL(*groupingGeneratorPointer,
                             setDistributionVector(groupings.size(), 1.0));

                aleatoric::GroupedRepetition(std::move(numberGenerator),
                                             std::move(groupingGenerator),
                                             range,
                                             groupings);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto numberGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto numberGeneratorPointer = numberGenerator.get();
        ALLOW_CALL(*numberGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*numberGeneratorPointer,
                   updateDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*numberGeneratorPointer,
                   updateDistributionVector(ANY(double)));
        ALLOW_CALL(*numberGeneratorPointer, getNumber()).RETURN(0);
        ALLOW_CALL(*numberGeneratorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        auto groupingGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto groupingGeneratorPointer = groupingGenerator.get();
        ALLOW_CALL(*groupingGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*groupingGeneratorPointer,
                   updateDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*groupingGeneratorPointer,
                   updateDistributionVector(ANY(double)));
        ALLOW_CALL(*groupingGeneratorPointer, getNumber()).RETURN(0);
        ALLOW_CALL(*groupingGeneratorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        aleatoric::Range range(11, 13);
        std::vector<int> groupings {2};

        aleatoric::GroupedRepetition instance(std::move(numberGenerator),
                                              std::move(groupingGenerator),
                                              range,
                                              groupings);

        WHEN("A number is requested")
        {
            THEN("It should return the generated number with the range offset "
                 "added")
            {
                int generatedNumber = 1;
                REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE(instance.getIntegerNumber() ==
                        generatedNumber + range.offset);
            }

            AND_WHEN("It is the first number requested and so the grouping "
                     "count is at 0")
            {
                // NB: grouping count is at 0 on initialisation

                THEN("It should call the grouping generator to get a number")
                {
                    // Number is used to select a grouping value from the
                    // groupings collection, by index
                    REQUIRE_CALL(*groupingGeneratorPointer, getNumber())
                        .RETURN(0);
                    instance.getIntegerNumber();
                }

                THEN("It updates the grouping generator to disallow the "
                     "generated number from being selected in future calls")
                {
                    int generatedNumber = 0;

                    REQUIRE_CALL(*groupingGeneratorPointer, getNumber())
                        .RETURN(generatedNumber);

                    REQUIRE_CALL(
                        *groupingGeneratorPointer,
                        updateDistributionVector(generatedNumber, 0.0));

                    instance.getIntegerNumber();
                }

                THEN("It should call the number generator to get a number")
                {
                    REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                        .RETURN(1);
                    instance.getIntegerNumber();
                }

                THEN("It updates the number generator to disallow the "
                     "generated number from being selected in future calls")
                {
                    int generatedNumber = 1;

                    REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                        .RETURN(generatedNumber);

                    REQUIRE_CALL(
                        *numberGeneratorPointer,
                        updateDistributionVector(generatedNumber, 0.0));

                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("A number is requested whilst a grouping is in place")
            {
                // This should select the first grouping which has the value 2
                REQUIRE_CALL(*groupingGeneratorPointer, getNumber()).RETURN(0);

                instance.getIntegerNumber(); // first call, calls the generators

                THEN("The generators should not be called to get new numbers")
                {
                    FORBID_CALL(*groupingGeneratorPointer, getNumber());
                    FORBID_CALL(*numberGeneratorPointer, getNumber());
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN(
                "A grouping has been exhausted, setting the count back to 0")
            {
                // This should select the first grouping which has the value 2
                REQUIRE_CALL(*groupingGeneratorPointer, getNumber()).RETURN(0);

                instance.getIntegerNumber(); // first call, calls the generators

                instance.getIntegerNumber(); // second call, should exhaust the
                                             // grouping and set the count to 0

                THEN("It should call the generators for a new grouping "
                     "and a new number selection")
                {
                    int generatedNumber = 0;

                    REQUIRE_CALL(*groupingGeneratorPointer, getNumber())
                        .RETURN(generatedNumber);
                    REQUIRE_CALL(
                        *groupingGeneratorPointer,
                        updateDistributionVector(generatedNumber, 0.0));

                    REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                        .RETURN(generatedNumber);
                    REQUIRE_CALL(
                        *numberGeneratorPointer,
                        updateDistributionVector(generatedNumber, 0.0));

                    instance.getIntegerNumber();
                }
            }

            THEN("It gets the state of the generators' distributions to "
                 "determine if the series are complete")
            {
                REQUIRE_CALL(*groupingGeneratorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                REQUIRE_CALL(*numberGeneratorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getIntegerNumber();
            }

            AND_WHEN("The series are complete")
            {
                THEN("It resets the generators' distributions")
                {
                    REQUIRE_CALL(*groupingGeneratorPointer,
                                 getDistributionVector())
                        .RETURN(std::vector<double> {0.0});
                    REQUIRE_CALL(*groupingGeneratorPointer,
                                 updateDistributionVector(1.0));
                    REQUIRE_CALL(*numberGeneratorPointer,
                                 getDistributionVector())
                        .RETURN(std::vector<double> {0.0});
                    REQUIRE_CALL(*numberGeneratorPointer,
                                 updateDistributionVector(1.0));
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The series are not complete")
            {
                THEN("It does not reset the generators' distributions")
                {
                    REQUIRE_CALL(*groupingGeneratorPointer,
                                 getDistributionVector())
                        .RETURN(std::vector<double> {1.0});
                    FORBID_CALL(*groupingGeneratorPointer,
                                updateDistributionVector(1.0));
                    REQUIRE_CALL(*numberGeneratorPointer,
                                 getDistributionVector())
                        .RETURN(std::vector<double> {1.0});
                    FORBID_CALL(*numberGeneratorPointer,
                                updateDistributionVector(1.0));
                    instance.getIntegerNumber();
                }
            }
        }

        WHEN("A reset is requested")
        {
            // NB: reset() also sets the count back to 0. The effect of the
            // count being at 0 is tested above, so will not be repeated here.

            THEN("It resets the generators' distributions")
            {
                REQUIRE_CALL(*groupingGeneratorPointer,
                             updateDistributionVector(1.0));
                REQUIRE_CALL(*numberGeneratorPointer,
                             updateDistributionVector(1.0));
                instance.reset();
            }
        }

        WHEN("The range is changed")
        {
            // NB: These tests use real dependencies
            aleatoric::GroupedRepetition realInstance(
                std::make_unique<aleatoric::DiscreteGenerator>(),
                std::make_unique<aleatoric::DiscreteGenerator>(),
                aleatoric::Range(11, 13),
                std::vector<int> {1});

            aleatoric::Range newRange(2, 10);

            realInstance.setRange(newRange);

            THEN("The returned range should match the received range")
            {
                auto returnedRange = realInstance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            AND_WHEN(
                "A full set of numbers matching the new range size is gathered")
            {
                // Note that the object is formed with only one grouping
                // possibility, which is 1. This makes it behave as Serial does,
                // which is fine for these tests
                std::vector<int> fullSet(newRange.size);
                for(auto &&i : fullSet) {
                    i = realInstance.getIntegerNumber();
                }

                THEN("All numbers in the set should fall within the new range")
                {
                    for(auto &&i : fullSet) {
                        REQUIRE(newRange.numberIsInRange(i));
                    }
                }

                THEN("Each number from the range should appear once in the set")
                {
                    for(int i = 0; i < newRange.size; i++) {
                        auto number = i + newRange.offset;
                        auto count =
                            std::count(fullSet.begin(), fullSet.end(), number);
                        REQUIRE(count == 1);
                    }
                }
            }
        }

        WHEN("range change: resetting the count when current grouping can be > "
             "1")
        {
            // This object has only one grouping of 2. When a
            // pre-range-change number is requested, followed by a range change
            // where the new range is entirely outside the old range, a grouping
            // count reset is needed otherwise the last number (which is now
            // outside the new range) will be repeated.

            aleatoric::GroupedRepetition realInstance(
                std::make_unique<aleatoric::DiscreteGenerator>(),
                std::make_unique<aleatoric::DiscreteGenerator>(),
                aleatoric::Range(11, 13),
                std::vector<int> {2});

            auto numberBefore = realInstance.getIntegerNumber();

            // change the range
            aleatoric::Range newRange(2, 10);
            realInstance.setRange(newRange);

            auto numberAfter = realInstance.getIntegerNumber();

            THEN("Number after range change should not match the number before "
                 "range change")
            {
                REQUIRE(numberAfter != numberBefore);
            }

            THEN("Number after range change should fall within new range")
            {
                REQUIRE(newRange.numberIsInRange(numberAfter));
            }
        }
    }
}
