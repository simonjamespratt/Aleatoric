#include "Geometric.hpp"

#include "Range.hpp"

#include <catch2/catch.hpp>

SCENARIO("TimeDomain::Geometric")
{
    GIVEN("The class is instantiated with an invalid range")
    {
        WHEN("The range start is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int collectionSize = 5;

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Geometric(actlib::Numbers::Range(0, 2),
                                                  collectionSize),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Geometric(actlib::Numbers::Range(0, 2),
                                                  collectionSize),
                    "The range object supplied must have a start value equal "
                    "to, or greater than, 1");
            }
        }
    }
    GIVEN("The class is instantiated such that whole number durations are "
          "produced")
    {
        actlib::Numbers::Range range(256, 4096);
        int collectionSize = 5;
        actlib::TimeDomain::Geometric instance(range, collectionSize);

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the collection size "
                 "specified at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == collectionSize);
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("The returned number should match the term at the point in "
                 "the expected geometric sequence")
            {
                // When the supplied parameters are:
                // range: 256 - 4096
                // collectionSize: 5
                // Then this will internally have a common ratio of 2 (see class
                // implementation for details). Therefore the geometric sequence
                // is calculated as: rangeStart * cr^n-term

                // And the sequence will be as follows:
                /*
                0: 256 * 2^0 = 256 (2^0 = 1)
                1: 256 * 2^1 = 512 (2^1 = 2)
                2: 256 * 2^2 = 1024 (2^2 = 4)
                3: 256 * 2^3 = 2048 (2^3 = 8)
                4: 256 * 2^4 = 4096 (2^4 = 16)
                */

                std::vector<int> expectedSequence {256, 512, 1024, 2048, 4096};

                for(int i = 0; i < expectedSequence.size(); i++) {
                    REQUIRE(instance.getDuration(i) == expectedSequence[i]);
                }
            }
        }
    }

    GIVEN("The class is instantiated such that durations require rounding")
    {
        actlib::Numbers::Range range(100, 1000);
        int collectionSize = 5;
        actlib::TimeDomain::Geometric instance(range, collectionSize);

        WHEN("Each duration is requested")
        {
            THEN("The returned number should match the term at the point in "
                 "the expected geometric sequence, having been rounded")
            {
                // When the supplied parameters are:
                // range: 100 - 1000
                // collectionSize: 5
                // Then this will internally have a common ratio
                // of:
                // 1.7782794100389228

                // Therefore the geometric sequence is calculated as:
                // rangeStart * cr^n-term

                // And the sequence will be as follows:
                /*
                0: 100 * 1.7782794100389228^0 = 100
                1: 100 * 1.7782794100389228^1 = 177.82794100389228 ~ 178
                2: 100 * 1.7782794100389228^2 = 316.22776601683793 ~ 316
                3: 100 * 1.7782794100389228^3 = 562.34132519034908 ~ 562
                4: 100 * 1.7782794100389228^4 = 1000
                */

                std::vector<int> expectedSequence {100, 178, 316, 562, 1000};

                for(int i = 0; i < expectedSequence.size(); i++) {
                    REQUIRE(instance.getDuration(i) == expectedSequence[i]);
                }
            }
        }
    }
}
