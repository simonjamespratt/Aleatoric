// NB: These tests are stripped-down tests in comparison to the Numbers
// Integration Tests. They will not interrogate the Protocols in the way that
// the Numbers Integretion Tests do, as that would lead to test duplication. We
// will simply check that the Collections Producers can work with the Protocols
// correctly.

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "DiscreteGenerator.hpp"
#include "NoRepetition.hpp"
#include "Numbers.hpp"
#include "Periodic.hpp"
#include "ProducerCollections.hpp"
#include "Serial.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

#include <catch2/catch.hpp>

SCENARIO("Collections: Integration using Basic")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated")
    {
        actlib::Collections::Producer<char> instance(
            source,
            factory.createBasic(0, source.size() - 1));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All the items in the collection should have been chosen at "
                 "least once")
            {
                for(auto &&i : source) {
                    auto findResult =
                        std::find(sample.begin(), sample.end(), i);
                    REQUIRE(findResult != sample.end());
                }
            }
        }
    }
}

SCENARIO("Collections: Integration using Cycle")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        WHEN("It is configured in the default state")
        {
            actlib::Collections::Producer<char> instance(
                source,
                factory.createCycle(0, source.size() - 1));

            AND_WHEN("A pair of cycles is requested")
            {
                std::vector<char> expectedResult {'a', 'b', 'c', 'a', 'b', 'c'};
                auto sample = instance.getCollection(expectedResult.size());

                THEN("The cycles should be as expected")
                {
                    REQUIRE(sample == expectedResult);
                }
            }
        }
    }
}

SCENARIO("Collections: Integration using Serial")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated")
    {
        actlib::Collections::Producer<char> instance(
            source,
            factory.createSerial(0, source.size() - 1));

        WHEN("A full series sample set has been gathered")
        {
            auto sample = instance.getCollection(source.size());

            THEN("The sample should include every item from the source "
                 "collection and only once")
            {
                for(auto &&i : source) {
                    int count = std::count(sample.begin(), sample.end(), i);
                    REQUIRE(count == 1);
                }
            }

            AND_WHEN("The next number is requested")
            {
                auto firstItemOfNextSet = instance.getItem();

                THEN("That number should appear in the previous set")
                {
                    auto numberAppears =
                        std::any_of(sample.begin(),
                                    sample.end(),
                                    [firstItemOfNextSet](int i) {
                                        return i == firstItemOfNextSet;
                                    });

                    REQUIRE(numberAppears);
                }
            }
        }
    }
}

SCENARIO("Collections: Integration using Precision")
{
    // TODO: Have to use a different collection size for this set of tests due
    // to an issue with the argument checking in Precision for summing the
    // values in the distribution. It should be set back to a size of 3
    // when this is fixed.
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c', 'd'};

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        WHEN("The distribution is uniform")
        {
            std::vector<double> distribution(source.size());

            // Make a uniform distribution
            for(auto &&i : distribution) {
                i = 1.0 / source.size();
            }

            actlib::Collections::Producer<char> instance(
                source,
                factory.createPrecision(0, source.size() - 1, distribution));

            AND_WHEN("A sample is requested")
            {
                auto sample = instance.getCollection(1000);

                THEN("All items in the sample should be from the source "
                     "collection")
                {
                    for(auto &&i : sample) {
                        REQUIRE_THAT(source, Catch::VectorContains(i));
                    }
                }
            }
        }
    }
}

SCENARIO("Collections: Integration with NoRepetition")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The producer has been instantiated")
    {
        actlib::Collections::Producer<char> instance(
            source,
            factory.createNoRepetition(0, source.size() - 1));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All items in the sample should be from the source collection")
            {
                for(auto &&i : sample) {
                    REQUIRE_THAT(source, Catch::VectorContains(i));
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

SCENARIO("Collections: Integration using Periodic")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        AND_GIVEN("The chance of repetition is mid range")
        {
            actlib::Collections::Producer<char> instance(
                source,
                factory.createPeriodic(0, source.size() - 1, 0.5));

            WHEN("A sample has been gathered")
            {
                auto sample = instance.getCollection(1000);

                THEN("All items in the sample should be from the source "
                     "collection")
                {
                    for(auto &&i : sample) {
                        REQUIRE_THAT(source, Catch::VectorContains(i));
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
    }
}

SCENARIO("Collections: Integration using AdjacentSteps")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        actlib::Collections::Producer<char> instance(
            source,
            factory.createAdjacentSteps(0, source.size() - 1));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All items in the sample should be from the source collection")
            {
                for(auto &&i : sample) {
                    REQUIRE_THAT(source, Catch::VectorContains(i));
                }
            }
        }
    }
}

SCENARIO("Collections: Integration using Walk")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c', 'd', 'e'};

    GIVEN("The Producer has been instantiated with no initial selection")
    {
        int maxStep = 2;
        actlib::Collections::Producer<char> instance(
            source,
            factory.createWalk(0, source.size() - 1, maxStep));

        WHEN("A sample has been gathered")
        {
            auto sample = instance.getCollection(1000);

            THEN("All items in the sample should be from the source collection")
            {
                for(auto &&i : sample) {
                    REQUIRE_THAT(source, Catch::VectorContains(i));
                }
            }
        }
    }
}
