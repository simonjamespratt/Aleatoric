#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "GroupedRepetition.hpp"
#include "NoRepetition.hpp"
#include "NumberProtocolFactory.hpp"
#include "NumbersProducer.hpp"
#include "Periodic.hpp"
#include "Serial.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

#include <algorithm> // std::adjacent_find, std::find
#include <array>
#include <catch2/catch.hpp>
#include <iostream>
#include <memory>

SCENARIO("Numbers: Integration using Basic")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(10, 20);

    GIVEN("The Producer has been instantiated")
    {
        aleatoric::NumbersProducer instance(factory.createBasic(0, 1),
                                            referenceRange);

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

SCENARIO("Numbers: Integration using Cycle")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 2);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: Not testing reset as it would just be a repeat of the unit tests.
        // Just testing the basics of the protocol functionality here

        WHEN("It is configured in the default state")
        {
            aleatoric::NumbersProducer instance(factory.createCycle(0, 1),
                                                referenceRange);

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
            aleatoric::NumbersProducer instance(
                factory.createCycle(0, 1, false, true),
                referenceRange);

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
            aleatoric::NumbersProducer instance(
                factory.createCycle(0, 1, true, false),
                referenceRange);

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
            aleatoric::NumbersProducer instance(
                factory.createCycle(0, 1, true, true),
                referenceRange);

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

SCENARIO("Numbers: Integration using Serial")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        aleatoric::NumbersProducer instance(factory.createSerial(0, 1),
                                            referenceRange);

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

        WHEN("A partial series sample set is gathered")
        {
            auto partialSample = instance.getIntegerCollection(3);

            AND_WHEN("A reset is made followed by the gathering of a full "
                     "series sample set")
            {
                instance.reset();

                auto fullSample = instance.getIntegerCollection(10);

                THEN("The full post-reset sample should include every number "
                     "from the range and only once")
                {
                    for(int i = 0; i < referenceRange.size; i++) {
                        int count =
                            std::count(fullSample.begin(), fullSample.end(), i);
                        REQUIRE(count == 1);
                    }
                }

                THEN("The numbers from the partial pre-reset sample set should "
                     "appear in the full post-reset sample set")
                {
                    for(auto &&psItem : partialSample) {
                        auto numberAppears = std::any_of(
                            fullSample.begin(),
                            fullSample.end(),
                            [psItem](int fsItem) { return psItem == fsItem; });

                        REQUIRE(numberAppears);
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Integration using Subset")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(1, 10);
    int subsetMin = 3;
    int subsetMax = 7;

    GIVEN("The Producer has been instantiated")
    {
        // TODO: DYNAMIC-PARAMS: because of coupling of range and subset min/max
        // in Subset protocol, have to set the range in protocol constructor to
        // the right range. When params can be updated all at once, this can go.
        aleatoric::NumbersProducer instance(
            factory.createSubset(1, 10, subsetMin, subsetMax),
            referenceRange);

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

SCENARIO("Numbers: Integration using GroupedRepetition")
{
    // NB: This is pretty hard to test entirely accurately, but I think the way
    // these tests are set up, it would result in sporadic failing tests, were
    // the serial nature of the selection of either range numbers or groupings
    // broken.

    // Testing reset() is not possible without going through a number of test
    // set ups where possible selections are constrained. Not going to bother
    // with that now, but could be done if desired. It would look similar to
    // Series tests above, but taking into consideration both range selection
    // and grouping selection.

    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(1, 2);
    std::vector<int> groupings {1, 2};
    int groupingsSum = 3; // sum of the above values

    GIVEN("The Producer has been instantiated")
    {
        aleatoric::NumbersProducer instance(
            factory.createGroupedRepetition(0, 1, groupings),
            referenceRange);

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

SCENARIO("Numbers: Integration using Ratio")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 4);

    GIVEN("The Producer has been instantiated with a Ratio Protocol with mixed "
          "ratio values")
    {
        std::vector<int> ratios {3, 1, 0, 2, 4};
        int ratiosSum = 10;

        // TODO: DYNAMIC-PARAMS: because of coupling of range and ratio
        // collection size in the Ratio protocol, have to set the range in
        // protocol constructor to the right range. When params can be updated
        // all at once, this can go.
        aleatoric::NumbersProducer instance(factory.createRatio(0, 4, ratios),
                                            referenceRange);

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

    GIVEN("The Producer has been instantiated with a Ratio Protocol with "
          "uniform ratio values of 1")
    {
        // NB: This allows the Serial nature of the Protocol to be tested

        std::vector<int> ratios {1, 1, 1, 1, 1};

        // TODO: DYNAMIC-PARAMS: because of coupling of range and ratio
        // collection size in the Ratio protocol, have to set the range in
        // protocol constructor to the right range. When params can be updated
        // all at once, this can go.
        aleatoric::NumbersProducer instance(factory.createRatio(0, 4, ratios),
                                            referenceRange);

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

        WHEN("A partial series sample set is gathered")
        {
            auto partialSample = instance.getIntegerCollection(3);

            AND_WHEN("A reset is made followed by the gathering of a full "
                     "series sample set")
            {
                instance.reset();

                auto fullSample =
                    instance.getIntegerCollection(referenceRange.size);

                THEN("The full post-reset sample should include every number "
                     "from the range and only once")
                {
                    for(int i = 0; i < referenceRange.size; i++) {
                        int count =
                            std::count(fullSample.begin(), fullSample.end(), i);
                        REQUIRE(count == 1);
                    }
                }

                THEN("The numbers from the partial pre-reset sample set should "
                     "appear in the full post-reset sample set")
                {
                    for(auto &&psItem : partialSample) {
                        auto numberAppears = std::any_of(
                            fullSample.begin(),
                            fullSample.end(),
                            [psItem](int fsItem) { return psItem == fsItem; });

                        REQUIRE(numberAppears);
                    }
                }
            }
        }
    }
}

SCENARIO("Numbers: Integration using Precision")
{
    // TODO: DOUBLE-SUMMING-PRECISION: Have to use a different range for this
    // set of tests due to an issue with the argument checking in Precision for
    // summing the values in the distribution. It should be set back to a range
    // of (0, 9) when this is fixed.
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 3);

    GIVEN("The Producer has been instantiated")
    {
        WHEN("The distribution is uniform")
        {
            std::vector<double> distribution(referenceRange.size);

            // Make a uniform distribution
            for(auto &&i : distribution) {
                i = 1.0 / referenceRange.size;
            }

            // TODO: DYNAMIC-PARAMS: because of coupling of range and
            // distribution collection size in the Precision protocol, have to
            // set the range in protocol constructor to the right range. When
            // params can be updated all at once, this can go.
            aleatoric::NumbersProducer instance(
                factory.createPrecision(0, 3, distribution),
                referenceRange);

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
            std::vector<double> distribution {1.0, 0.0, 0.0, 0.0};

            // TODO: DYNAMIC-PARAMS: because of coupling of range and
            // distribution collection size in the Precision protocol, have to
            // set the range in protocol constructor to the right range. When
            // params can be updated all at once, this can go.
            aleatoric::NumbersProducer instance(
                factory.createPrecision(0, 3, distribution),
                referenceRange);

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

SCENARIO("Numbers: Integration using NoRepetition")
{
    // NB: No tests for reset() as this is hard to test. What would you test
    // for, seeing as all the numbers (except the last selected) have equal
    // proability of selection?
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 9);

    aleatoric::NumbersProducer instance(factory.createNoRepetition(0, 1),
                                        referenceRange);

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

SCENARIO("Numbers: Integration using Periodic")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: reset() is not testable as you cannot gaurantee that the next
        // number will be either the same as, or different from, the last

        AND_GIVEN("The chance of repetition is mid range")
        {
            aleatoric::NumbersProducer instance(
                factory.createPeriodic(0, 1, 0.5),
                referenceRange);

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
            aleatoric::NumbersProducer instance(
                factory.createPeriodic(0, 1, 0.0),
                referenceRange);

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
            aleatoric::NumbersProducer instance(
                factory.createPeriodic(0, 1, 1.0),
                referenceRange);

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

SCENARIO("Numbers: Integration using AdjacentSteps")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the adjacent steps functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be either a 4 or a 6.

        aleatoric::NumbersProducer instance(factory.createAdjacentSteps(0, 1),
                                            referenceRange);

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

SCENARIO("Numbers: Integration using Walk")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the walk functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be within the max step range.

        int maxStep = 3;
        // TODO: DYNAMIC-PARAMS: because of coupling of range and
        // maxStep collection size in the Walk protocol, have to
        // set the range in protocol constructor to the right range. When
        // params can be updated all at once, this can go.
        aleatoric::NumbersProducer instance(factory.createWalk(0, 9, maxStep),
                                            referenceRange);

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

SCENARIO("Numbers: Integration using GranularWalk")
{
    aleatoric::NumberProtocolFactory factory;
    aleatoric::Range referenceRange(0, 10);
    aleatoric::UniformGenerator generator;

    GIVEN("The Producer has been instantiated")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the GranularWalk functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be within the deviation factor range.

        double deviationFactor = 0.3;
        double dfRange = 3.0;

        aleatoric::NumbersProducer instance(
            factory.createGranularWalk(0, 1, deviationFactor),
            referenceRange);

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
