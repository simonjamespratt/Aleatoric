// NB: The Time Domain Producer takes both a duration protocol and a number
// protocol. These tests concentrate on testing the duration protocols in-depth
// and do not put much emphasis on the number protocols. This is because
// duration protocols are not put through extensive integration tests anywhere
// else, whereas number protocols are. So going in-depth with number protocols
// would just be a duplication of the work done in the NumbersIntegrationTests.

// In fact, these tests use just the Cycle number protocol as a way to get the
// items from the duration protocols in a way that is uniform across all
// duration protocols under test. It is used because it is the simplest and most
// predictable of all the number protocols.

#include "DurationsProducer.hpp"
#include "Geometric.hpp"
#include "Multiples.hpp"
#include "NumberProtocolFactory.hpp"
#include "Prescribed.hpp"
#include "Range.hpp"
#include "UniformGenerator.hpp"

#include <catch2/catch.hpp>

SCENARIO("TimeDomain: Integration using Prescribed and Cycle")
{
    aleatoric::NumberProtocolFactory numbersFactory;

    std::vector<int> sourceDurations {1, 2, 3, 4, 5};

    aleatoric::Prescribed durationProtocol(sourceDurations);

    aleatoric::DurationsProducer instance(
        durationProtocol,
        numbersFactory.createCycle(0, sourceDurations.size() - 1));

    WHEN("A sample has been gathered that matches the size of the duration "
         "collection")
    {
        auto sample = instance.getCollection(sourceDurations.size());

        THEN("The result should match the provided source durations collection")
        {
            REQUIRE(sample == sourceDurations);
        }
    }
}

SCENARIO("TimeDomain: Integration using Multiples and Cycle")
{
    aleatoric::NumberProtocolFactory numbersFactory;

    GIVEN("The duration protocol is configured with a range and no deviation "
          "factor")
    {
        int baseIncrement = 100;
        aleatoric::Range range(11, 13);

        aleatoric::Multiples durationProcotol(baseIncrement, range);

        // TODO: DYNAMIC-PARAMS: This is a good example of the reason why it is
        // difficult for a caller to get the number protocol configured with the
        // correct range: The caller needs to know that the number protocol must
        // be configured for selecting INDICES that match the produced internal
        // durations collection of the duration protocol, rather than matching
        // the range provided to the duration protocol.
        aleatoric::DurationsProducer instance(
            durationProcotol,
            numbersFactory.createCycle(0, range.size - 1));

        WHEN("A sample has been gathered that matches the size of the range")
        {
            auto sample = instance.getCollection(range.size);

            THEN("The sample should include each increment in the range "
                 "multiplied by the base increment")
            {
                std::vector<int> expectedResult {1100, 1200, 1300};
                REQUIRE(sample == expectedResult);
            }
        }
    }

    GIVEN("The duration protocol is configured with a range a deviation factor")
    {
        int baseIncrement = 100;
        double deviationFactor = 0.1;
        aleatoric::Range range(10, 20);

        // TODO: TIME DOMAIN: this is a lot for a caller to have to set up. Is
        // there a need for a factory here? Don't think a caller should have to
        // worry about sending in a Uniform Generator in order to create an
        // instance of a duration protocol.
        aleatoric::Multiples durationProtocol(
            baseIncrement,
            range,
            deviationFactor,
            std::make_unique<aleatoric::UniformGenerator>());

        // TODO: DYNAMIC-PARAMS: same as above
        aleatoric::DurationsProducer instance(
            durationProtocol,
            numbersFactory.createCycle(0, range.size - 1));

        WHEN("A sample has been gathered that matches the size of the range")
        {
            auto sample = instance.getCollection(range.size);

            THEN("The sample should include a number ± the deviation factor of "
                 "each increment in the range, first having been multiplied by "
                 "the base increment")
            {
                std::vector<int> expectedResultWithoutDeviation {1000,
                                                                 1100,
                                                                 1200,
                                                                 1300,
                                                                 1400,
                                                                 1500,
                                                                 1600,
                                                                 1700,
                                                                 1800,
                                                                 1900,
                                                                 2000};

                for(int i = 0; i < sample.size(); i++) {
                    auto expectedItem = expectedResultWithoutDeviation[i];

                    auto deviation = expectedItem * deviationFactor;

                    auto minDeviation = expectedItem - deviation;

                    auto maxDeviation = expectedItem + deviation;

                    REQUIRE(sample[i] >= minDeviation);
                    REQUIRE(sample[i] <= maxDeviation);
                }
            }
        }
    }

    GIVEN("The duration protocol is configured with a multipliers collection "
          "and no deviation factor")
    {
        int baseIncrement = 100;
        std::vector<int> multipliers {10, 11, 12};

        aleatoric::Multiples durationProtocol(baseIncrement, multipliers);

        // TODO: DYNAMIC-PARAMS: same as above
        aleatoric::DurationsProducer instance(
            durationProtocol,
            numbersFactory.createCycle(0, multipliers.size() - 1));

        WHEN("A sample has been gathered that matches the size of the "
             "multiples collection")
        {
            auto sample = instance.getCollection(multipliers.size());

            THEN("The sample should include each item in the multiples "
                 "collection multiplied by the base increment")
            {
                std::vector<int> expectedResult {1000, 1100, 1200};

                REQUIRE(sample == expectedResult);
            }
        }
    }

    GIVEN("The duration protocol is configured with a multipliers collection "
          "a deviation factor")
    {
        int baseIncrement = 100;
        std::vector<int>
            multipliers {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        double deviationFactor = 0.1;

        // TODO: TIME DOMAIN: same issue as above
        aleatoric::Multiples durationProtocol(
            baseIncrement,
            multipliers,
            deviationFactor,
            std::make_unique<aleatoric::UniformGenerator>());

        // TODO: DYNAMIC-PARAMS: same as above
        aleatoric::DurationsProducer instance(
            durationProtocol,
            numbersFactory.createCycle(0, multipliers.size() - 1));

        WHEN("A sample has been gathered that matches the size of the "
             "multiples collection")
        {
            auto sample = instance.getCollection(multipliers.size());

            THEN("The sample should include a number ± the deviation factor of "
                 "each increment in the multipliers collection, first having "
                 "been multiplied by the base increment")
            {
                std::vector<int> expectedResultWithoutDeviation {1000,
                                                                 1100,
                                                                 1200,
                                                                 1300,
                                                                 1400,
                                                                 1500,
                                                                 1600,
                                                                 1700,
                                                                 1800,
                                                                 1900,
                                                                 2000};

                for(int i = 0; i < sample.size(); i++) {
                    auto expectedItem = expectedResultWithoutDeviation[i];

                    auto deviation = expectedItem * deviationFactor;

                    auto minDeviation = expectedItem - deviation;

                    auto maxDeviation = expectedItem + deviation;

                    REQUIRE(sample[i] >= minDeviation);
                    REQUIRE(sample[i] <= maxDeviation);
                }
            }
        }
    }
}

SCENARIO("TimeDomain: Integration using Geometric and Cycle")
{
    aleatoric::NumberProtocolFactory numbersFactory;

    GIVEN("The duration protocol is configured with a range and a collection "
          "size")
    {
        aleatoric::Range range(256, 4096);
        int collectionSize = 5;

        aleatoric::Geometric durationProtocol(range, collectionSize);

        // TODO: DYNAMIC-PARAMS: same as above
        aleatoric::DurationsProducer instance(
            durationProtocol,
            numbersFactory.createCycle(0, range.size - 1));

        WHEN("A sample is gathered that is the size of the collection size "
             "argument")
        {
            auto sample = instance.getCollection(collectionSize);

            THEN("The sample should should match the expected geometric "
                 "sequence")
            {
                // NB: The following is lifted directly from the unit tests for
                // Geometric!

                // When the supplied parameters are:
                // range: 256 - 4096
                // collectionSize: 5
                // Then this will internally have a
                // common ratio of 2 (see class implementation for details).
                // Therefore the geometric sequence is calculated as: rangeStart
                // * cr^n-term

                // And the sequence will be as follows:
                /*
                0: 256 * 2^0 = 256 (2^0 = 1)
                1: 256 * 2^1 = 512 (2^1 = 2)
                2: 256 * 2^2 = 1024 (2^2 = 4)
                3: 256 * 2^3 = 2048 (2^3 = 8)
                4: 256 * 2^4 = 4096 (2^4 = 16)
                */

                std::vector<int> expectedSequence {256, 512, 1024, 2048, 4096};

                REQUIRE(sample == expectedSequence);
            }
        }
    }
}
