#include "UniformGenerator.hpp"

#include <catch2/catch.hpp>

SCENARIO("UniformGenerator")
{
    using namespace aleatoric;

    GIVEN("The class is instantiated with a default constructor")
    {
        UniformGenerator instance;

        WHEN("A number is requested")
        {
            THEN("It should produce random numbers within the default range of "
                 "0 to 1")
            {
                // NB: This is a pseudo test, in that it is unlikely to be
                // wrong, but is not guaranteed to be right! In order to test
                // this properly it would require understanding how to
                // deterministically seed the RNG or Mock things (which I don't
                // think is possible)
                for(int i = 0; i < 100; i++) {
                    int number = instance.getNumber();
                    REQUIRE(number >= 0);
                    REQUIRE(number <= 1);
                }
            }
        }
    }

    GIVEN("The class is instantiated with a defined range")
    {
        UniformGenerator instance(1, 2);

        WHEN("A number is requested")
        {
            THEN("It should produce random numbers within provided the range")
            {
                // NB: This is a pseudo test, in that it is unlikely to be
                // wrong, but is not guaranteed to be right! In order to test
                // this properly it would require understanding how to
                // deterministically seed the RNG or Mock things (which I don't
                // think is possible)
                for(int i = 0; i < 100; i++) {
                    int number = instance.getNumber();
                    REQUIRE(number >= 1);
                    REQUIRE(number <= 2);
                }
            }
        }

        WHEN("The distribution is changed")
        {
            instance.setDistribution(2, 3);

            AND_WHEN("A number is requested")
            {
                THEN("It should produce random numbers within provided the "
                     "range")
                {
                    // NB: This is a pseudo test, in that it is unlikely to be
                    // wrong, but is not guaranteed to be right! In order to
                    // test this properly it would require understanding how to
                    // deterministically seed the RNG or Mock things (which I
                    // don't think is possible)
                    for(int i = 0; i < 100; i++) {
                        int number = instance.getNumber();
                        REQUIRE(number >= 2);
                        REQUIRE(number <= 3);
                    }
                }
            }
        }
    }
}
