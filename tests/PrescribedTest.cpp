#include "Prescribed.hpp"

#include <catch2/catch.hpp>
#include <stdexcept>

SCENARIO("TimeDomain::Prescribed")
{
    GIVEN("The class is instantiated with an invalid duration")
    {
        std::vector<int> sourceWithInvalidDuration {0, 100, 200};

        WHEN("A duration is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                REQUIRE_THROWS_AS(aleatoric::Prescribed(sourceWithInvalidDuration),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Prescribed(sourceWithInvalidDuration),
                    "All durations supplied must be equal to, or greater than, "
                    "1");
            }
        }
    }
    GIVEN("The class is instantiated")
    {
        std::vector<int> source {100, 200, 300};
        aleatoric::Prescribed instance(source);

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the size of the source "
                 "provided at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == source.size());
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("It should match the value at each index of the source given "
                 "at construction")
            {
                for(int i = 0; i < source.size(); i++) {
                    REQUIRE(instance.getDuration(i) == source[i]);
                }
            }
        }
    }
}
