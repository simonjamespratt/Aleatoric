#include "Serial.hpp"
#include "DiscreteGenerator.hpp"
#include "Engine.hpp"
#include "Range.hpp"
#include <array>
#include <catch2/catch.hpp>

// TODO: This needs to be instantiated ONCE for ALL tests to use,
// ideally in the main.cpp of the tests
// It also needs to be handled by a smart pointer or Engine::Instance() needs to
// return a smart pointer
actlib::RNG::Engine *engine = actlib::RNG::Engine::Instance();

SCENARIO("Numbers::Serial") {
    GIVEN("The class is invoked with an inclusive range of 3") {
        actlib::Numbers::Range range(1, 3);
        actlib::Numbers::DiscreteGenerator generator(
            engine->getEngine(), range.size, 1.0);
        actlib::Numbers::Serial instance(generator, range);
        int firstNumber = instance.getNumber();
        int secondNumber = instance.getNumber();
        int thirdNumber = instance.getNumber();
        int fourthNumber = instance.getNumber();

        WHEN("The first number is requested") {
            THEN("The number returned should be within the range specified") {
                REQUIRE(firstNumber >= 1);
                REQUIRE(firstNumber <= 3);
            }
        }

        WHEN("The second number is requested") {
            THEN("The number returned should be any within the range "
                 "specified") {
                REQUIRE(secondNumber >= 1);
                REQUIRE(secondNumber <= 3);
            }

            THEN("The number returned should not be the first number "
                 "requested") {
                REQUIRE_FALSE(secondNumber == firstNumber);
            }
        }

        WHEN("The third number is requested") {
            THEN("The number returned should be any within the range "
                 "specified") {
                REQUIRE(thirdNumber >= 1);
                REQUIRE(thirdNumber <= 3);
            }

            THEN("The number returned should not be the first or second number "
                 "requested") {
                REQUIRE_FALSE(thirdNumber == firstNumber);
                REQUIRE_FALSE(thirdNumber == secondNumber);
            }
        }

        WHEN("The fourth number is requested") {
            THEN("The number returned should be any within the range "
                 "specified") {
                REQUIRE(fourthNumber >= 1);
                REQUIRE(fourthNumber <= 3);
            }

            THEN("[reset] The probability should have reset to be equal and "
                 "number returned should be one of the previously selected "
                 "numbers") {
                std::array<int, 3> previouslySelectedNumbers = {
                    firstNumber, secondNumber, thirdNumber};
                bool result = std::any_of(
                    previouslySelectedNumbers.begin(),
                    previouslySelectedNumbers.end(),
                    [fourthNumber](int i) { return i == fourthNumber; });
                REQUIRE(result == true);
            }
        }
    }
}
