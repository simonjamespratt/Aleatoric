#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "NoRepetition.hpp"
#include "Numbers.hpp"
#include "Periodic.hpp"
#include "ProducerGranular.hpp"
#include "ProducerSteps.hpp"
#include "Serial.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

#include <algorithm> // std::adjacent_find, std::find
#include <array>
#include <catch2/catch.hpp>
#include <memory>

actlib::Numbers::Numbers factory;

SCENARIO("Numbers: Integration using Basic")
{
    actlib::Numbers::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        actlib::Numbers::Steps::Producer instance(
            factory.createBasic(referenceRange.start, referenceRange.end));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All the numbers of the sample should fall within the "
                 "spacified range")
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
                    auto findResult =
                        std::find(sample.begin(), sample.end(), i);
                    REQUIRE(findResult != sample.end());
                }
            }
        }
    }
}

SCENARIO("Numbers: Integration using Serial")
{
    actlib::Numbers::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated")
    {
        actlib::Numbers::Steps::Producer instance(
            factory.createSerial(referenceRange.start, referenceRange.end));

        WHEN("A full series sample set has been gathered")
        {
            auto sample = instance.getCollection(10);

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
                auto firstNumberOfNextSet = instance.getNumber();

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
            auto partialSample = instance.getCollection(3);

            AND_WHEN("A reset is made followed by the gathering of a full "
                     "series sample set")
            {
                instance.reset();

                auto fullSample = instance.getCollection(10);

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

SCENARIO("Numbers: Integration using NoRepetition")
{
    // NB: No tests for reset() as this is hard to test. What would you test
    // for, seeing as all the numbers (except the last selected) have equal
    // proability of selection?

    actlib::Numbers::Range referenceRange(0, 9);

    actlib::Numbers::Steps::Producer instance(
        factory.createNoRepetition(referenceRange.start, referenceRange.end));

    GIVEN("The Producer has been instantiated")
    {
        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

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
    actlib::Numbers::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: reset() is not testable as you cannot gaurantee that the next
        // number will be either the same as, or different from, the last

        AND_GIVEN("The chance of repetition is mid range")
        {
            actlib::Numbers::Steps::Producer instance(
                factory.createPeriodic(referenceRange.start,
                                       referenceRange.end,
                                       0.5));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getCollection(1000);

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
            actlib::Numbers::Steps::Producer instance(
                factory.createPeriodic(referenceRange.start,
                                       referenceRange.end,
                                       0.0));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getCollection(1000);

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
            actlib::Numbers::Steps::Producer instance(
                factory.createPeriodic(referenceRange.start,
                                       referenceRange.end,
                                       1.0));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getCollection(1000);

                THEN("Only one number should appear within the sample")
                {
                    for(auto &&i : sample) {
                        REQUIRE(i == *sample.begin());
                    }
                }
            }
        }
    }

    GIVEN("The Producer has been instantiated with an initial selection")
    {
        // NB: chance of repetition is not important for these tests
        int initialSelection = 5;
        actlib::Numbers::Steps::Producer instance(
            factory.createPeriodic(referenceRange.start,
                                   referenceRange.end,
                                   0.5,
                                   initialSelection));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("The first number produced should be the initial selection")
            {
                REQUIRE(sample[0] == initialSelection);
            }

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= referenceRange.start);
                    REQUIRE(i <= referenceRange.end);
                }
            }
        }

        AND_WHEN("Following a reset, a number is requested")
        {
            instance.getNumber();
            instance.reset();
            auto nextNumberAfterReset = instance.getNumber();

            THEN("The first number produced should be the initial selection")
            {
                REQUIRE(nextNumberAfterReset == initialSelection);
            }
        }
    }
}

SCENARIO("Numbers: Integration using AdjacentSteps")
{
    int rangeStart = 0;
    int rangeEnd = 9;

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the adjacent steps functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be either a 4 or a 6.

        actlib::Numbers::Steps::Producer instance(
            factory.createAdjacentSteps(rangeStart, rangeEnd));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= rangeStart);
                    REQUIRE(i <= rangeEnd);
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

    GIVEN("The Producer has been instantiated with an initial selection")
    {
        int initialSelection = 5;

        actlib::Numbers::Steps::Producer instance(
            factory.createAdjacentSteps(rangeStart,
                                        rangeEnd,
                                        initialSelection));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("The first number produced should be the initial selection")
            {
                REQUIRE(sample[0] == initialSelection);
            }

            THEN("All numbers of the sample should fall within the specified "
                 "range")
            {
                for(auto &&i : sample) {
                    REQUIRE(i >= rangeStart);
                    REQUIRE(i <= rangeEnd);
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

            AND_WHEN("Following a reset")
            {
                instance.getNumber();
                instance.reset();
                auto producedNumber = instance.getNumber();
                THEN(
                    "The first number produced should be the initial selection")
                {
                    REQUIRE(producedNumber == initialSelection);
                }
            }
        }
    }
}

SCENARIO("Numbers: Integration using Walk")
{
    actlib::Numbers::Range referenceRange(0, 9);

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the walk functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be within the max step range.

        int maxStep = 3;
        actlib::Numbers::Steps::Producer instance(
            factory.createWalk(referenceRange.start,
                               referenceRange.end,
                               maxStep));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

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

    GIVEN("The Producer has been instantiated with an initial selection")
    {
        int maxStep = 3;
        int initialSelection = 5;
        actlib::Numbers::Steps::Producer instance(
            factory.createWalk(referenceRange.start,
                               referenceRange.end,
                               maxStep,
                               initialSelection));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("The first number produced should be the initial selection")
            {
                REQUIRE(sample[0] == initialSelection);
            }

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

            AND_WHEN("Following a reset")
            {
                instance.getNumber();
                instance.reset();
                auto producedNumber = instance.getNumber();
                THEN(
                    "The first number produced should be the initial selection")
                {
                    REQUIRE(producedNumber == initialSelection);
                }
            }
        }
    }
}

SCENARIO("Numbers: Integration using GranularWalk")
{
    actlib::Numbers::Range referenceRange(0, 10);
    actlib::Numbers::UniformGenerator generator;

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        // NB: No tests for reset() where an initial number selection has not
        // been made because it could lead to sporadic test failures. There is
        // no sure way to test that the next number after reset is not following
        // the GranularWalk functionality. For example, if the last number
        // before a reset is 5, there is nothing to say that the first number
        // after reset won't be within the deviation factor range.

        double deviationFactor = 0.3;
        double dfRange = 3.0;

        actlib::Numbers::Granular::Producer instance(
            factory.createGranularWalk(referenceRange.start,
                                       referenceRange.end,
                                       deviationFactor));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

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

    GIVEN("The Producer has been instantiated with an initial selection")
    {
        double deviationFactor = 0.3;
        double dfRange = 3.0;
        int initialSelection = 5;

        actlib::Numbers::Granular::Producer instance(
            factory.createGranularWalk(referenceRange.start,
                                       referenceRange.end,
                                       deviationFactor,
                                       initialSelection));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("The first number produced should be the initial selection")
            {
                REQUIRE(sample[0] == initialSelection);
            }

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
                        REQUIRE(current >= prev - dfRange);
                        REQUIRE(current <= prev + dfRange);
                    }
                }
            }

            AND_WHEN("Following a reset")
            {
                instance.getNumber();
                instance.reset();
                auto producedNumber = instance.getNumber();
                THEN(
                    "The first number produced should be the initial selection")
                {
                    REQUIRE(producedNumber == initialSelection);
                }
            }
        }
    }
}
