#include "Periodic.hpp"
#include "DiscreteGeneratorMock.hpp"
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Periodic") {
    DiscreteGeneratorMock generator;
    std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};
    ALLOW_CALL(generator, getDistributionVector())
        .RETURN(initialGeneratorDistributionState);
    ALLOW_CALL(generator, setDistributionVector(ANY(std::vector<double>)));

    actlib::Numbers::Range range(1, 3);

    GIVEN(
        "The class is instantiated with an invalid chanceOfRepetition value") {
        WHEN("The value provided is greater than 1.0") {
            THEN("A standard invalid_argument exeception is thrown") {
                REQUIRE_THROWS_AS(
                    actlib::Numbers::Periodic(generator, range, 1.1),
                    std::invalid_argument);
                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Periodic(generator, range, 1.1),
                    "The value passed as argument for chanceOfRepetition must "
                    "be within the range of 0.0 - 1.0");
            }
        }

        WHEN("The value provided is less than 0.0") {
            THEN("A standard invalid_argument exeception is thrown") {
                REQUIRE_THROWS_AS(
                    actlib::Numbers::Periodic(generator, range, -1.0),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Periodic(generator, range, 1.1),
                    "The value passed as argument for chanceOfRepetition must "
                    "be within the range of 0.0 - 1.0");
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection") {
        double chanceOfRepetition = 0.5;
        actlib::Numbers::Periodic instance(
            generator, range, chanceOfRepetition);

        int generatedNumber = 1; // mid range selection

        WHEN("A number is requested") {
            THEN("It returns a generated number with the range offset added") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            AND_THEN("The generator distibution should be set with a bias in "
                     "favour of the last generated number according to the "
                     "periodicity received during construction") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                REQUIRE_CALL(generator, getDistributionVector())
                    .TIMES(1) // doesn't seem to register that it is called in
                              // the constructor as well
                    .RETURN(initialGeneratorDistributionState);

                // The total of all values in the vector must equal 1.0.
                // The value at the index of the last selected number
                // must have the value of the periodicity (chanceOfRepetition).
                // The remainder of 1.0 - periodicity is shared equally amongst
                // the remaining vector indices.
                REQUIRE_CALL(generator,
                             setDistributionVector(std::vector<double> {
                                 0.25, chanceOfRepetition, 0.25}));
                instance.getNumber();
            }
        }

        WHEN("A reset is performed") {
            THEN("The generator distribution is set to uniform - equal "
                 "probability of selection for each number in the range upon "
                 "next call to getNumber()") {
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }

    GIVEN("The class is instantiated with an invalid initialSelection value") {
        WHEN("The value provided is greater than the range end") {
            THEN("A standard invalid_argument exception is thrown") {
                REQUIRE_THROWS_AS(actlib::Numbers::Periodic(
                                      generator, range, 0.5, (range.end + 1)),
                                  std::invalid_argument);
                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Periodic(
                        generator, range, 0.5, (range.end + 1)),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }

        WHEN("The value provided is less than the range start") {
            THEN("A standard invalid_argument exception is thrown") {
                REQUIRE_THROWS_AS(actlib::Numbers::Periodic(
                                      generator, range, 0.5, (range.start - 1)),
                                  std::invalid_argument);
                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Periodic(
                        generator, range, 0.5, (range.start - 1)),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }
    }

    GIVEN("The class is instantiated with an initial number selection") {
        double chanceOfRepetition = 0.5;
        int initialSelection = 1; // start of range selection
        actlib::Numbers::Periodic instance(
            generator, range, chanceOfRepetition, initialSelection);

        WHEN("The first number is requested") {
            THEN("The initial number selection is returned") {
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            THEN("The generator should not have been called to get a number") {
                FORBID_CALL(generator, getNumber());
                instance.getNumber();
            }

            AND_THEN("The generator distribution should be set with a bias in "
                     "favour of the initial number selection according to the "
                     "periodicity received during construction") {
                REQUIRE_CALL(generator, getDistributionVector())
                    .RETURN(initialGeneratorDistributionState);

                // The index in the distribution vector for the initial
                // selection is equal to the initial selection, LESS, the range
                // offset
                REQUIRE_CALL(generator,
                             setDistributionVector(
                                 std::vector<double> {0.5, 0.25, 0.25}));
                instance.getNumber();
            }
        }

        WHEN("A subsequent number is requested") {
            instance.getNumber();    // initial call to get number
            int generatedNumber = 2; // end of range selection
            THEN("The generated number is returned with the range offset "
                 "added") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            AND_THEN("The generator distribution is set in favour of this "
                     "generated number, according to the periodicity received "
                     "during construction") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                REQUIRE_CALL(generator, getDistributionVector())
                    .RETURN(std::vector<double> {0.5, 0.25, 0.25});
                REQUIRE_CALL(generator,
                             setDistributionVector(
                                 std::vector<double> {0.25, 0.25, 0.5}));
                instance.getNumber();
            }
        }

        WHEN("A reset is performed following a previous call to get a number") {
            ALLOW_CALL(generator, updateDistributionVector(1.0));
            instance.getNumber(); // unsets the initial state
            AND_WHEN("The next number is requested") {
                THEN("The initial selection is returned, due to the initial "
                     "state having been reinstated during the reset") {
                    instance.reset();
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
