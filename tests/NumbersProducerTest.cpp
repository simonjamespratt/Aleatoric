#include "NumbersProducer.hpp"

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "GroupedRepetition.hpp"
#include "NoRepetition.hpp"
#include "Periodic.hpp"
#include "Serial.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

#include <algorithm> // std::adjacent_find, std::find
#include <array>
#include <catch2/catch.hpp>
#include <iostream>
#include <memory>

SCENARIO("Numbers: Using Basic")
{
    using namespace aleatoric;

    Range referenceRange(10, 20);

    GIVEN("The Producer has been instantiated")
    {
        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::basic));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(BasicParams())));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getIntegerCollection(1000);

            THEN("All the numbers of the sample should fall within the "
                 "specified range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("All possible numbers within the range should have been "
                 "chosen at least once")
            {
                for(int i = 0; i < referenceRange.size; i++) {
                    REQUIRE_THAT(
                        sample,
                        Catch::VectorContains(i + referenceRange.offset));
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Cycle")
{
    using namespace aleatoric;

    Range referenceRange(0, 2);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        WHEN("It is configured in the default state")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::cycle));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(CycleParams(false, false))));

            AND_WHEN("A pair of cycles is requested")
            {
                std::vector<int> expectedResult {0, 1, 2, 0, 1, 2};
                auto sample =
                    instance.getIntegerCollection(expectedResult.size());

                THEN("The cycles should be as expected")
                {
                    REQUIRE(sample == expectedResult);
                }
            }
        }

        WHEN("It is configured in the reverse, unidirectional mode")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::cycle));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(CycleParams(false, true))));

            AND_WHEN("A pair of cycles is requested")
            {
                std::vector<int> expectedResult {2, 1, 0, 2, 1, 0};
                auto sample =
                    instance.getIntegerCollection(expectedResult.size());

                THEN("The cycles should be as expected")
                {
                    REQUIRE(sample == expectedResult);
                }
            }
        }

        WHEN("It is configured in the bidirectional mode")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::cycle));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(CycleParams(true, false))));

            AND_WHEN("A pair of cycles is requested")
            {
                std::vector<int> expectedResult {0, 1, 2, 1, 0, 1, 2, 1, 0};
                auto sample =
                    instance.getIntegerCollection(expectedResult.size());

                THEN("The cycles should be as expected")
                {
                    REQUIRE(sample == expectedResult);
                }
            }
        }

        WHEN("It is configured in the bidirectionla, reverse mode")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::cycle));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(CycleParams(true, true))));

            AND_WHEN("A pair of cycles is requested")
            {
                std::vector<int> expectedResult {2, 1, 0, 1, 2, 1, 0, 1, 2};
                auto sample =
                    instance.getIntegerCollection(expectedResult.size());

                THEN("The cycles should be as expected")
                {
                    REQUIRE(sample == expectedResult);
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Serial")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::serial));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(SerialParams())));

        WHEN("A full series sample set has been gathered")
        {
            auto sample = instance.getIntegerCollection(10);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("The sample should include every number from the range and "
                 "only once")
            {
                for(int i = 0; i < referenceRange.size; i++) {
                    int count = std::count(sample.begin(), sample.end(), i);
                    REQUIRE(count == 1);
                }
            }

            AND_WHEN("The next number is requested")
            {
                auto firstNumberOfNextSet = instance.getIntegerNumber();

                THEN("That number should appear in the previous set")
                {
                    auto numberAppears =
                        std::any_of(sample.begin(),
                                    sample.end(),
                                    [firstNumberOfNextSet](int i) {
                                        return i == firstNumberOfNextSet;
                                    });

                    REQUIRE(numberAppears);
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Subset")
{
    using namespace aleatoric;

    Range referenceRange(1, 10);
    int subsetMin = 3;
    int subsetMax = 7;

    GIVEN("The Producer has been instantiated")
    {
        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::subset));

        instance.setParams(NumberProtocolConfig(
            referenceRange,
            NumberProtocolParams(SubsetParams(subsetMin, subsetMax))));

        WHEN("A sample has been collected")
        {
            auto sample = instance.getIntegerCollection(1000);

            std::vector<int> countResults;

            for(int i = 0; i < referenceRange.size; i++) {
                auto count = std::count(sample.begin(),
                                        sample.end(),
                                        i + referenceRange.offset);

                countResults.push_back(count);
            }

            THEN("The count for each number from the range existing within the "
                 "sample should be between the subset min and max inclusive")
            {
                int countOfExistingNumbers = 0;

                for(auto &&i : countResults) {
                    if(i > 0) {
                        countOfExistingNumbers++;
                    }
                }

                REQUIRE(countOfExistingNumbers >= subsetMin);
                REQUIRE(countOfExistingNumbers <= subsetMax);
            }

            THEN("Sum of all found count results should be the same as the "
                 "sample range, hence all numbers in the sample are from the "
                 "range")
            {
                int tally = std::accumulate(countResults.begin(),
                                            countResults.end(),
                                            0);
                REQUIRE(tally == sample.size());
            }
        }
    }
}

SCENARIO("Numbers: Using GroupedRepetition")
{
    using namespace aleatoric;

    // NB: This is pretty hard to test entirely accurately, but I think the way
    // these tests are set up, it would result in sporadic failing tests, were
    // the serial nature of the selection of either range numbers or groupings
    // broken.

    Range referenceRange(1, 2);
    std::vector<int> groupings {1, 2};
    int groupingsSum = 3; // sum of the above values

    GIVEN("The Producer has been instantiated")
    {
        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::groupedRepetition));

        instance.setParams(NumberProtocolConfig(
            referenceRange,
            NumberProtocolParams(GroupedRepetitionParams(groupings))));

        WHEN("Two samples each consisting of a full series set has been "
             "gathered")
        {
            // NB: because the sizes of the range and the groupings are the
            // same, their serial sets will match

            auto sampleOne = instance.getIntegerCollection(groupingsSum);
            auto sampleTwo = instance.getIntegerCollection(groupingsSum);

            std::vector<std::vector<int>> possibleResults {{1, 2, 2},
                                                           {1, 1, 2},
                                                           {2, 1, 1},
                                                           {2, 2, 1}};

            THEN("Each number from the range should be present in the form of "
                 "one of the possible groupings")
            {
                REQUIRE_THAT(sampleOne,
                             Catch::Equals(possibleResults[0]) ||
                                 Catch::Equals(possibleResults[1]) ||
                                 Catch::Equals(possibleResults[2]) ||
                                 Catch::Equals(possibleResults[3]));

                REQUIRE_THAT(sampleTwo,
                             Catch::Equals(possibleResults[0]) ||
                                 Catch::Equals(possibleResults[1]) ||
                                 Catch::Equals(possibleResults[2]) ||
                                 Catch::Equals(possibleResults[3]));
            }
        }
    }
}

SCENARIO("Numbers: Using Ratio")
{
    using namespace aleatoric;

    Range referenceRange(0, 4);

    GIVEN("The Producer is set with a Ratio Protocol with mixed "
          "ratio values")
    {
        std::vector<int> ratios {3, 1, 0, 2, 4};
        int ratiosSum = 10;

        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::ratio));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(RatioParams(ratios))));

        WHEN("A full series sample set has been gathered")
        {
            auto sample = instance.getIntegerCollection(ratiosSum);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("The sample should contain the right amount of each number "
                 "within the given range")
            {
                for(int i = 0; i < referenceRange.size; i++) {
                    auto count = std::count(sample.begin(),
                                            sample.end(),
                                            i + referenceRange.offset);
                    REQUIRE(count == ratios[i]);
                }
            }
        }
    }

    GIVEN("The Producer is set with a Ratio Protocol with "
          "uniform ratio values of 1")
    {
        // NB: This allows the Serial nature of the Protocol to be tested

        std::vector<int> ratios {1, 1, 1, 1, 1};

        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::ratio));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(RatioParams(ratios))));

        WHEN("A full series sample set has been gathered")
        {
            auto sample = instance.getIntegerCollection(referenceRange.size);

            THEN("The sample should include every number from the range and "
                 "only once")
            {
                for(int i = 0; i < referenceRange.size; i++) {
                    int count = std::count(sample.begin(), sample.end(), i);
                    REQUIRE(count == 1);
                }
            }

            AND_WHEN("The next number is requested")
            {
                auto firstNumberOfNextSet = instance.getIntegerNumber();

                THEN("That number should appear in the previous set")
                {
                    auto numberAppears =
                        std::any_of(sample.begin(),
                                    sample.end(),
                                    [firstNumberOfNextSet](int i) {
                                        return i == firstNumberOfNextSet;
                                    });

                    REQUIRE(numberAppears);
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Precision")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        WHEN("The distribution is uniform")
        {
            std::vector<double> distribution(referenceRange.size);

            // Make a uniform distribution
            for(auto &&i : distribution) {
                i = 1.0 / referenceRange.size;
            }

            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::precision));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PrecisionParams(distribution))));

            AND_WHEN("A sample is requested")
            {
                auto sample = instance.getIntegerCollection(1000);

                THEN("All values in the sample should fall within the range")
                {
                    for(auto &&i : sample) {
                        REQUIRE(i >= referenceRange.start);
                        REQUIRE(i <= referenceRange.end);
                    }
                }
            }
        }

        WHEN("The distribution favours one number in the range")
        {
            // Biased distribution in favour of a certain number (0)
            std::vector<double> distribution {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::precision));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PrecisionParams(distribution))));

            AND_WHEN("A sample is requested")
            {
                auto sample = instance.getIntegerCollection(1000);

                THEN("The sample should only contain the favoured number")
                {
                    for(auto &&i : sample) {
                        REQUIRE(i == 0);
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Using NoRepetition")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    NumbersProducer instance(
        NumberProtocol::create(NumberProtocol::Type::noRepetition));

    instance.setParams(
        NumberProtocolConfig(referenceRange,
                             NumberProtocolParams(NoRepetitionParams())));

    GIVEN("The Producer has been instantiated")
    {
        WHEN("A sample has been gathered")
        {
            auto sample = instance.getIntegerCollection(1000);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("There should be no direct or immediate repetition of numbers")
            {
                for(auto i = sample.begin(); i != sample.end(); ++i) {
                    // Don't make assertion for first number in collection as
                    // there is no previous number to compare with
                    if(i != sample.begin()) {
                        REQUIRE(*i != *std::prev(i));
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Periodic")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        AND_GIVEN("The chance of repetition is mid range")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::periodic));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PeriodicParams(0.5))));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getIntegerCollection(1000);

                THEN("All numbers of the sample should fall within the "
                     "specified range")
                {
                    for(auto &&i : sample) {
                        REQUIRE(i >= referenceRange.start);
                        REQUIRE(i <= referenceRange.end);
                    }
                }

                THEN("At least one number should be directly repeated")
                {
                    auto firstIndexOfFoundMatchingAdjacentValues =
                        std::adjacent_find(sample.begin(), sample.end());
                    REQUIRE(firstIndexOfFoundMatchingAdjacentValues !=
                            sample.end());
                }
            }
        }

        AND_GIVEN("There is no chance of repetition")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::periodic));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PeriodicParams(0.0))));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getIntegerCollection(1000);

                THEN("No direct repetition of numbers should occur")
                {
                    auto firstIndexOfFoundMatchingAdjacentValues =
                        std::adjacent_find(sample.begin(), sample.end());
                    REQUIRE(firstIndexOfFoundMatchingAdjacentValues ==
                            sample.end());
                }

                THEN("All possible numbers within the range should have been "
                     "chosen at least once")
                {
                    for(int i = 0; i < referenceRange.size; i++) {
                        auto findResult =
                            std::find(sample.begin(), sample.end(), i);
                        REQUIRE(findResult != sample.end());
                    }
                }
            }
        }

        AND_GIVEN("There can only be the chance of repetition")
        {
            NumbersProducer instance(
                NumberProtocol::create(NumberProtocol::Type::periodic));

            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PeriodicParams(1.0))));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getIntegerCollection(1000);

                THEN("Only one number should appear within the sample")
                {
                    for(auto &&i : sample) {
                        REQUIRE(i == *sample.begin());
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Using AdjacentSteps")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::adjacentSteps));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(AdjacentStepsParams())));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getIntegerCollection(1000);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("Each number should be directly adjacent - either side - to "
                 "the last")
            {
                for(auto i = sample.begin(); i != sample.end(); ++i) {
                    // Don't make assertion for first number in collection as
                    // there is no previous number to compare with
                    if(i != sample.begin()) {
                        auto current = *i;
                        auto prev = *std::prev(i);
                        REQUIRE(
                            ((current == prev + 1) || (current == prev - 1)));
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Using Walk")
{
    using namespace aleatoric;

    Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        int maxStep = 3;

        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::walk));

        instance.setParams(
            NumberProtocolConfig(referenceRange,
                                 NumberProtocolParams(WalkParams(maxStep))));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getIntegerCollection(1000);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("Each number should be within the max step range of the last")
            {
                for(auto i = sample.begin(); i != sample.end(); ++i) {
                    // Don't make assertion for first number in collection as
                    // there is no previous number to compare with
                    if(i != sample.begin()) {
                        auto current = *i;
                        auto prev = *std::prev(i);
                        REQUIRE(current >= prev - maxStep);
                        REQUIRE(current <= prev + maxStep);
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Using GranularWalk")
{
    using namespace aleatoric;

    Range referenceRange(0, 10);
    UniformGenerator generator;

    GIVEN("The Producer has been instantiated")
    {
        double deviationFactor = 0.3;
        double dfRange = 3.0;

        NumbersProducer instance(
            NumberProtocol::create(NumberProtocol::Type::granularWalk));

        instance.setParams(NumberProtocolConfig(
            referenceRange,
            NumberProtocolParams(GranularWalkParams(deviationFactor))));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getDecimalCollection(1000);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }

            THEN("Each number should be within the range created by the "
                 "deviation factor of the last number")
            {
                for(auto i = sample.begin(); i != sample.end(); ++i) {
                    // Don't make assertion for first number in collection as
                    // there is no previous number to compare with
                    if(i != sample.begin()) {
                        auto current = *i;
                        auto prev = *std::prev(i);
                        REQUIRE(current >= prev - dfRange);
                        REQUIRE(current <= prev + dfRange);
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Get and set params using Cycle")
{
    using namespace aleatoric;

    Range referenceRange(0, 2);

    NumbersProducer instance(
        NumberProtocol::create(NumberProtocol::Type::cycle));

    instance.setParams(
        NumberProtocolConfig(referenceRange,
                             NumberProtocolParams(CycleParams(false, false))));

    GIVEN("Params have not been changed from initial setting")
    {
        WHEN("Get params")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            auto cycleParams = params.protocols.getCycle();

            THEN("Params should reflect correct state")
            {
                REQUIRE(returnedRange.start == referenceRange.start);
                REQUIRE(returnedRange.end == referenceRange.end);
                REQUIRE(params.protocols.getActiveProtocol() ==
                        NumberProtocol::Type::cycle);
                REQUIRE_FALSE(cycleParams.getBidirectional());
                REQUIRE_FALSE(cycleParams.getReverseDirection());
            }
        }

        WHEN("Request pair of cycles")
        {
            std::vector<int> expectedResult {0, 1, 2, 0, 1, 2};
            auto sample = instance.getIntegerCollection(expectedResult.size());

            THEN("Result should be as expected")
            {
                REQUIRE(sample == expectedResult);
            }
        }
    }

    GIVEN("Params have been updated: change to reverse unidirectional and new "
          "range")
    {
        Range newRange(1, 5);
        NumberProtocolConfig newParams(
            newRange,
            NumberProtocolParams(CycleParams(false, true)));

        instance.setParams(newParams);

        THEN("Params reflect update")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            auto cycleParams = params.protocols.getCycle();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
            REQUIRE_FALSE(cycleParams.getBidirectional());
            REQUIRE(cycleParams.getReverseDirection());
        }

        WHEN("A pair of cycles is requested")
        {
            std::vector<int> expectedResult {5, 4, 3, 2, 1, 5, 4, 3, 2, 1};
            auto sample = instance.getIntegerCollection(expectedResult.size());

            THEN("Result is the reverse of the original sample")
            {
                REQUIRE(sample == expectedResult);
            }
        }
    }

    WHEN("User provides the wrong protocol params")
    {
        THEN("Exception is thrown")
        {
            // Provides Basic protocol params, not cycle
            REQUIRE_THROWS_AS(instance.setParams(NumberProtocolConfig(
                                  Range(20, 30),
                                  NumberProtocolParams(BasicParams()))),
                              std::invalid_argument);
        }
    }
}

SCENARIO("Numbers: Change protocol")
{
    using namespace aleatoric;

    Range referenceRange(1, 3);

    NumbersProducer instance(
        NumberProtocol::create(NumberProtocol::Type::cycle));

    WHEN("Before protocol change")
    {
        THEN("Active protocol is as expected")
        {
            auto activeProtocol =
                instance.getParams().protocols.getActiveProtocol();
            REQUIRE(activeProtocol ==
                    NumberProtocol::Type::cycle);
        }

        THEN("Set of numbers is as expected")
        {
            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(CycleParams(false, false))));

            std::vector<int> expectedResult {1, 2, 3, 1, 2, 3};
            auto set = instance.getIntegerCollection(expectedResult.size());

            REQUIRE(set == expectedResult);
        }
    }

    WHEN("After protocol change")
    {
        instance.setProtocol(
            NumberProtocol::create(NumberProtocol::Type::periodic));

        THEN("Active protocol is as expected")
        {
            auto activeProtocol =
                instance.getParams().protocols.getActiveProtocol();
            REQUIRE(activeProtocol ==
                    NumberProtocol::Type::periodic);
        }

        THEN("Set of numbers is as expected")
        {
            instance.setParams(NumberProtocolConfig(
                referenceRange,
                NumberProtocolParams(PeriodicParams(1.0))));

            // use as reference number for set gathered next
            auto firstNumber = instance.getIntegerNumber();

            for(int i = 0; i < 1000; i++) {
                REQUIRE(instance.getIntegerNumber() == firstNumber);
            }
        }
    }
}
