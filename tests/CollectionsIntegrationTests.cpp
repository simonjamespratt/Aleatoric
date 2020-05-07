// NB: These tests are stripped-down tests in comparison to the Numbers
// Integration Tests. They will not interrogate the Protocols in the way that
// the Numbers Integretion Tests do, as that would lead to test duplication. We
// will simply check that the Collections Producers can work with the Protocols
// correctly.

#include "Numbers.hpp"
#include "ProducerCollections.hpp"

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

SCENARIO("Collections: Integration using Subset")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c', 'd', 'e'};
    int subsetMin = 2;
    int subsetMax = 4;

    GIVEN("The Producer is instantiated")
    {
        actlib::Collections::Producer<char> instance(
            source,
            factory.createSubset(0, source.size() - 1, subsetMin, subsetMax));

        WHEN("A sample has been collected")
        {
            auto sample = instance.getCollection(1000);

            std::vector<int> countResults;

            for(auto &&i : source) {
                auto count = std::count(sample.begin(), sample.end(), i);

                countResults.push_back(count);
            }

            THEN("The count for each item from the source existing within the "
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
                 "sample range, hence all items in the sample are from the "
                 "source")
            {
                int tally = std::accumulate(countResults.begin(),
                                            countResults.end(),
                                            0);
                REQUIRE(tally == sample.size());
            }
        }
    }
}

SCENARIO("Collections: Integration using GroupedRepetition")
{
    // NB: See the scenario in Numbers Integration Tests for GroupedRepetition
    // re. limitations of these tests.

    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b'};
    std::vector<int> groupings {1, 2};
    int groupingsSum = 3; // sum of the above values

    GIVEN("The Producer has been instantiated")
    {
        // NB: because the sizes of the source and the groupings are the
        // same, the serial sets in GroupedRepetition will match

        actlib::Collections::Producer<char> instance(
            source,
            factory.createGroupedRepetition(0, source.size() - 1, groupings));

        WHEN("Two samples each consisting of a full series set has been "
             "gathered")
        {
            auto sampleOne = instance.getCollection(groupingsSum);
            auto sampleTwo = instance.getCollection(groupingsSum);

            std::vector<std::vector<char>> possibleResults {{'a', 'b', 'b'},
                                                            {'a', 'a', 'b'},
                                                            {'b', 'a', 'a'},
                                                            {'b', 'b', 'a'}};

            THEN("Each source element should be present in the form of on of "
                 "the possible groupings")
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

SCENARIO("Collections: Integration using Ratio")
{
    actlib::Numbers::Numbers factory;
    std::vector<char> source {'a', 'b', 'c'};

    GIVEN("The Producer has been instantiated with a Ratio Protocol with mixed "
          "ratio values")
    {
        // TODO: DYNAMIC-PARAMS: A lot of things need to be in alignment here,
        // all currently required to be coordinated by the caller: ratio.size(),
        // range.size (and values) and source.size() must all be aligned
        std::vector<int> ratios {3, 1, 2};
        int ratioSum = 6;

        actlib::Collections::Producer<char> instance(
            source,
            factory.createRatio(0, source.size() - 1, ratios));

        WHEN("A full series sample has been gathered")
        {
            auto sample = instance.getCollection(ratioSum);

            THEN("The sample should contain the right amount of each source "
                 "item")
            {
                for(int i = 0; i < source.size(); i++) {
                    auto count =
                        std::count(sample.begin(), sample.end(), source[i]);
                    REQUIRE(count == ratios[i]);
                }
            }
        }
    }
}

SCENARIO("Collections: Integration using Precision")
{
    // TODO: DOUBLE-SUMMING-PRECISION: Have to use a different collection size
    // for this set of tests due to an issue with the argument checking in
    // Precision for summing the values in the distribution. It should be set
    // back to a size of 3 when this is fixed.
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
