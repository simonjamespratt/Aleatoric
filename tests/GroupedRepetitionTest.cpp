#include "GroupedRepetition.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::GroupedRepetition")
{
    GIVEN("The class is being constructed")
    {
        auto numberGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto numberGeneratorPointer = numberGenerator.get();
        ALLOW_CALL(*numberGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));

        auto groupingGenerator = std::make_unique<DiscreteGeneratorMock>();
        auto groupingGeneratorPointer = groupingGenerator.get();
        ALLOW_CALL(*groupingGeneratorPointer,
                   setDistributionVector(ANY(int), ANY(double)));

        auto range = std::make_unique<actlib::Numbers::Range>(11, 13);
        auto rangePointer = range.get();

        std::vector<int> groupings {1, 2, 3};

        WHEN("In the constructor")
        {
            THEN("The number generator should be set to the size of the range "
                 "with an equal probability setting")
            {
                REQUIRE_CALL(*numberGeneratorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                actlib::Numbers::Steps::GroupedRepetition(
                    std::move(numberGenerator),
                    std::move(groupingGenerator),
                    std::move(range),
                    groupings);
            }

            THEN("The grouping generator should be set to the size of the "
                 "groupings collection with an equal probability setting")
            {
                REQUIRE_CALL(*groupingGeneratorPointer,
                             setDistributionVector(groupings.size(), 1.0));

                actlib::Numbers::Steps::GroupedRepetition(
                    std::move(numberGenerator),
                    std::move(groupingGenerator),
                    std::move(range),
                    groupings);
            }
        }
    }

    GIVEN("The class is instantiated")
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

        auto range = std::make_unique<actlib::Numbers::Range>(11, 13);
        auto rangePointer = range.get();

        std::vector<int> groupings {2};

        actlib::Numbers::Steps::GroupedRepetition instance(
            std::move(numberGenerator),
            std::move(groupingGenerator),
            std::move(range),
            groupings);

        WHEN("A number is requested")
        {
            THEN("It should return the generated number with the range offset "
                 "added")
            {
                int generatedNumber = 1;
                REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE(instance.getNumber() ==
                        generatedNumber + rangePointer->offset);
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
                    instance.getNumber();
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

                    instance.getNumber();
                }

                THEN("It should call the number generator to get a number")
                {
                    REQUIRE_CALL(*numberGeneratorPointer, getNumber())
                        .RETURN(1);
                    instance.getNumber();
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

                    instance.getNumber();
                }
            }

            AND_WHEN("A number is requested whilst a grouping is in place")
            {
                // This should select the first grouping which has the value 2
                REQUIRE_CALL(*groupingGeneratorPointer, getNumber()).RETURN(0);

                instance.getNumber(); // first call, calls the generators

                THEN("The generators should not be called to get new numbers")
                {
                    FORBID_CALL(*groupingGeneratorPointer, getNumber());
                    FORBID_CALL(*numberGeneratorPointer, getNumber());
                    instance.getNumber();
                }
            }

            AND_WHEN(
                "A grouping has been exhausted, setting the count back to 0")
            {
                // This should select the first grouping which has the value 2
                REQUIRE_CALL(*groupingGeneratorPointer, getNumber()).RETURN(0);

                instance.getNumber(); // first call, calls the generators

                instance.getNumber(); // second call, should exhaust the
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

                    instance.getNumber();
                }
            }

            THEN("It gets the state of the generators' distributions to "
                 "determine if the series are complete")
            {
                REQUIRE_CALL(*groupingGeneratorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                REQUIRE_CALL(*numberGeneratorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getNumber();
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
                    instance.getNumber();
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
                    instance.getNumber();
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
    }
}
