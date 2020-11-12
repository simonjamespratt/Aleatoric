#include "GroupedRepetition.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::GroupedRepetition: default constructor")
{
    using namespace aleatoric;

    GroupedRepetition instance(std::make_unique<DiscreteGenerator>(),
                               std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto groupings = params.protocols.getGroupedRepetition().getGroupings();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(groupings == std::vector<int> {1});
    }

    THEN("Set of numbers should be as per basic serial process")
    {
        // Only one grouping of 1 makes it function as though it were a basic
        // serial process

        std::vector<std::vector<int>> possibleResults {{0, 1}, {1, 0}};
        std::vector<std::vector<int>> set;

        for(int i = 0; i < 1000; i++) {
            std::vector<int> pair(2);
            for(auto &&i : pair) {
                i = instance.getIntegerNumber();
            }
            set.push_back(pair);
        }

        for(auto &&pair : set) {
            REQUIRE_THAT(pair,
                         Catch::Equals(possibleResults[0]) ||
                             Catch::Equals(possibleResults[1]));
        }
    }
}

SCENARIO("Numbers::GroupedRepetition")
{
    using namespace aleatoric;

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

        Range range(11, 13);
        std::vector<int> groupings {1, 2, 3};

        WHEN("In the constructor")
        {
            THEN("The number generator should be set to the size of the range "
                 "with an equal probability setting")
            {
                REQUIRE_CALL(*numberGeneratorPointer,
                             setDistributionVector(range.size, 1.0));
                GroupedRepetition(std::move(numberGenerator),
                                  std::move(groupingGenerator),
                                  range,
                                  groupings);
            }

            THEN("The grouping generator should be set to the size of the "
                 "groupings collection with an equal probability setting")
            {
                REQUIRE_CALL(*groupingGeneratorPointer,
                             setDistributionVector(groupings.size(), 1.0));

                GroupedRepetition(std::move(numberGenerator),
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

        Range range(11, 13);
        std::vector<int> groupings {2};

        GroupedRepetition instance(std::move(numberGenerator),
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
    }
}

SCENARIO("Numbers::GroupedRepetition: params")
{
    using namespace aleatoric;

    std::vector<int> groupings {3, 4};
    GroupedRepetition instance(std::make_unique<DiscreteGenerator>(),
                               std::make_unique<DiscreteGenerator>(),
                               Range(20, 30),
                               groupings);

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("Will reflect state of object")
        {
            REQUIRE(returnedRange.start == 20);
            REQUIRE(returnedRange.end == 30);
            REQUIRE(params.protocols.getGroupedRepetition().getGroupings() ==
                    groupings);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocolParameters::Protocols::ActiveProtocol::
                        groupedRepetition);
        }
    }

    WHEN("Set params")
    {
        Range newRange(1, 2);
        std::vector<int> newGroupings {2};
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::GroupedRepetition(newGroupings)));

        THEN("object is updated")
        {
            instance.setParams(newParams);
            auto params = instance.getParams();
            auto returnedRange = params.getRange();

            REQUIRE(params.protocols.getGroupedRepetition().getGroupings() ==
                    newGroupings);
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }

        THEN("A set should match expected result")
        {
            instance.setParams(newParams);

            std::vector<int> set(4);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            REQUIRE_THAT(set,
                         Catch::Equals(std::vector<int> {1, 1, 2, 2}) ||
                             Catch::Equals(std::vector<int> {2, 2, 1, 1}));
        }

        AND_WHEN("A grouping count reset is required")
        {
            // When a pre-range-change number is requested, followed by a range
            // change where the new range is entirely outside the old range, a
            // grouping count reset is needed otherwise the last number (which
            // is now outside the new range) will be repeated.

            instance.getIntegerNumber();

            instance.setParams(newParams);

            auto numberAfter = instance.getIntegerNumber();

            THEN("number after should be within new range")
            {
                REQUIRE(newRange.numberIsInRange(numberAfter));
            }
        }
    }
}
