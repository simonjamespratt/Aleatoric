#ifndef ConcreteProtocolMock_hpp
#define ConcreteProtocolMock_hpp

#include "ProtocolSteps.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class NumbersStepsProtocolMock : public actlib::Numbers::Steps::Protocol {
  public:
    MAKE_MOCK0(getNumber, int());
    MAKE_MOCK0(reset, void());
};

#endif /* ConcreteProtocolMock_hpp */
