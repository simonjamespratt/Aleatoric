#include "ProducerTimeDomain.hpp"

#include "DurationProtocolMock.hpp"
#include "NumbersStepsProtocolMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("TimeDomain::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        DurationProtocolMock durationProtocol;

        auto numberProtocol = std::make_unique<NumbersStepsProtocolMock>();
        auto numberProtocolPointer = numberProtocol.get();

        actlib::TimeDomain::Producer instance(durationProtocol,
                                              std::move(numberProtocol));

        WHEN("A duration is requested")
        {
            THEN("It should get a number from the number protocol and use it "
                 "as an index to get a duration from the duration protocol and "
                 "return it")
            {
                int index = 1;
                int returnedDurationAtIndex = 2;

                REQUIRE_CALL(*numberProtocolPointer, getNumber()).RETURN(index);

                REQUIRE_CALL(durationProtocol, getDuration(index))
                    .RETURN(returnedDurationAtIndex);

                auto duration = instance.getDuration();

                REQUIRE(duration == returnedDurationAtIndex);
            }
        }

        WHEN("A collection of durations is requested")
        {
            THEN("It should return a collection of the specified size "
                 "containing durations from the duration protocol whose "
                 "indices match the result of calling the number protocol to "
                 "get those indices")
            {
                int index = 1;
                int returnedDurationAtIndex = 2;

                REQUIRE_CALL(*numberProtocolPointer, getNumber())
                    .TIMES(100)
                    .RETURN(index);

                REQUIRE_CALL(durationProtocol, getDuration(index))
                    .TIMES(100)
                    .RETURN(returnedDurationAtIndex);

                auto collection = instance.getCollection(100);
                REQUIRE(collection.size() == 100);

                for(auto &&i : collection) {
                    REQUIRE(i == returnedDurationAtIndex);
                }
            }
        }

        WHEN("A reset is requested")
        {
            THEN("It should reset the number protocol")
            {
                REQUIRE_CALL(*numberProtocolPointer, reset());
                instance.reset();
            }
        }
    }
}
