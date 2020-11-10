#include "Range.hpp"

#include <catch2/catch.hpp>

SCENARIO("Range")
{
    GIVEN("Construction")
    {
        WHEN("The supplied end value is less than or equal to the supplied "
             "start value")
        {
            THEN("An invalid_argument exception should be thrown")
            {
                REQUIRE_THROWS_AS(aleatoric::Range(2, 1),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(aleatoric::Range(2, 1),
                                    "The supplied range end must be greater "
                                    "than the range start");

                REQUIRE_THROWS_AS(aleatoric::Range(1, 1),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(aleatoric::Range(1, 1),
                                    "The supplied range end must be greater "
                                    "than the range start");
            }
        }
    }

    GIVEN("The object is constructed")
    {
        aleatoric::Range range(2, 8);

        AND_GIVEN("A number is being checked for being within range")
        {
            WHEN("The number is in range")
            {
                THEN("It returns true")
                {
                    REQUIRE(range.numberIsInRange(4));
                }
            }

            WHEN("The number is below the range")
            {
                REQUIRE_FALSE(range.numberIsInRange(1));
            }

            WHEN("The number is above the range")
            {
                REQUIRE_FALSE(range.numberIsInRange(9));
            }
        }

        AND_GIVEN("A floating point number is checked for being in range")
        {
            WHEN("Is in range")
            {
                THEN("Returns true")
                {
                    REQUIRE(range.floatingPointIsInRange(5.12));
                }
            }

            WHEN("Below range")
            {
                THEN("Returns false")
                {
                    REQUIRE_FALSE(range.floatingPointIsInRange(1.999999));
                }
            }

            WHEN("Above range")
            {
                THEN("Returns false")
                {
                    REQUIRE_FALSE(range.floatingPointIsInRange(8.0000001));
                }
            }
        }
    }
}
