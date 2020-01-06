#include "AdjacentSteps.hpp"
#include "DiscreteGeneratorMock.hpp"
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::AdjacentSteps") {
    DiscreteGeneratorMock generator;
    ALLOW_CALL(generator, updateDistributionVector(ANY(double)));
    ALLOW_CALL(generator, updateDistributionVector(ANY(int), 1.0));

    GIVEN("The class is instantiated WITHOUT an initial number selection") {
        int generatedNumber = 1;
        actlib::Numbers::Range range(1, 3);
        actlib::Numbers::AdjacentSteps instance(generator, range);

        WHEN("A number is requested") {
            THEN("It returns a generated number with the range offset added") {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            AND_WHEN("The returned number is mid range") {
                auto midRangeGeneratedNumber = 2 - range.offset;
                auto stepUp = midRangeGeneratedNumber + 1;
                auto stepDown = midRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can be "
                     "taken either up or down") {
                    REQUIRE_CALL(generator, getNumber())
                        .RETURN(midRangeGeneratedNumber);
                    REQUIRE_CALL(generator, updateDistributionVector(0.0));
                    REQUIRE_CALL(generator,
                                 updateDistributionVector(stepUp, 1.0));
                    REQUIRE_CALL(generator,
                                 updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(
                        generator,
                        updateDistributionVector(midRangeGeneratedNumber, 1.0));
                    instance.getNumber();
                }
            }

            AND_WHEN("The returned number is the bottom (start) of the range") {
                auto startOfRangeGeneratedNumber = range.start - range.offset;
                auto stepUp = startOfRangeGeneratedNumber + 1;
                auto stepDown = startOfRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can "
                     "only be taken upwards") {
                    REQUIRE_CALL(generator, getNumber())
                        .RETURN(startOfRangeGeneratedNumber);
                    REQUIRE_CALL(generator, updateDistributionVector(0.0));
                    REQUIRE_CALL(generator,
                                 updateDistributionVector(stepUp, 1.0));
                    FORBID_CALL(generator,
                                updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(generator,
                                updateDistributionVector(
                                    startOfRangeGeneratedNumber, 1.0));
                    instance.getNumber();
                }
            }

            AND_WHEN("The returned number is the top (end) of the range") {
                auto endOfRangeGeneratedNumber = range.end - range.offset;
                auto stepUp = endOfRangeGeneratedNumber + 1;
                auto stepDown = endOfRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can "
                     "only be taken downwards") {
                    REQUIRE_CALL(generator, getNumber())
                        .RETURN(endOfRangeGeneratedNumber);
                    REQUIRE_CALL(generator, updateDistributionVector(0.0));
                    FORBID_CALL(generator,
                                updateDistributionVector(stepUp, 1.0));
                    REQUIRE_CALL(generator,
                                 updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(generator,
                                updateDistributionVector(
                                    endOfRangeGeneratedNumber, 1.0));
                    instance.getNumber();
                }
            }
        }

        WHEN("A reset is made") {
            THEN("The generator distribution is set to uniform (equal "
                 "probability") {
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                instance.reset();
            }

            AND_WHEN("The next number is requested") {
                THEN("It calls the generator to get one and returns it with "
                     "the range offset added") {
                    REQUIRE_CALL(generator, getNumber())
                        .RETURN(generatedNumber);
                    instance.reset();
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == generatedNumber + range.offset);
                }
            }
        }
    }

    GIVEN("The class is instantiated WITH an initial number selection") {
        actlib::Numbers::Range range(1, 3);
        int initialSelection = 2; // a mid-range selection (within range)
        actlib::Numbers::AdjacentSteps instance(
            generator, range, initialSelection);

        WHEN("A number is first requested") {
            THEN("The given initial selection is returned and no number is "
                 "generated") {
                FORBID_CALL(generator, getNumber());
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            AND_THEN("The stepwise distribution logic is run, having stripped "
                     "the range offset from the provided initial selection") {
                REQUIRE_CALL(generator, updateDistributionVector(0.0));
                REQUIRE_CALL(generator,
                             updateDistributionVector(
                                 (initialSelection - range.offset) - 1, 1.0));
                REQUIRE_CALL(generator,
                             updateDistributionVector(
                                 (initialSelection - range.offset) + 1, 1.0));
                FORBID_CALL(generator,
                            updateDistributionVector(
                                (initialSelection - range.offset), 1.0));
                instance.getNumber();
            }
        }

        WHEN("A subsequent number is requested") {
            int generatedNumber = 1;
            instance.getNumber(); // first call

            THEN("It returns a generated number with the range offset added") {
                REQUIRE_CALL(generator, getNumber())
                    .TIMES(1)
                    .RETURN(generatedNumber);

                auto secondCallResult = instance.getNumber(); // subsequent call
                REQUIRE(secondCallResult == generatedNumber + range.offset);
            }

            AND_THEN("The stepwise distribution logic is run") {
                REQUIRE_CALL(generator, getNumber())
                    .TIMES(1)
                    .RETURN(generatedNumber);
                REQUIRE_CALL(generator, updateDistributionVector(0.0)).TIMES(1);
                REQUIRE_CALL(generator, updateDistributionVector(ANY(int), 1.0))
                    .TIMES(1);
                instance.getNumber(); // subsequent call
            }
        }

        WHEN("A reset is made") {
            THEN("The generator distribution is set to uniform (equal "
                 "probability") {
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                instance.reset();
            }

            AND_WHEN("The next number is requested") {
                THEN("It returns the initial number provided on instantiation "
                     "and does not call the generator") {
                    FORBID_CALL(generator, getNumber());
                    instance.getNumber(); // this call makes sure to switch the
                                          // state of _haveRequestedFirstNumber
                                          // to true

                    instance.reset(); // this resets _haveRequestedFirstNumber
                                      // to false
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
