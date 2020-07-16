#ifndef NumberProtocolMock_hpp
#define NumberProtocolMock_hpp

#include "NumberProtocol.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class NumberProtocolMock : public aleatoric::NumberProtocol {
  public:
    MAKE_MOCK0(getIntegerNumber, int());
    MAKE_MOCK0(getDecimalNumber, double());
    MAKE_MOCK0(reset, void());
    MAKE_MOCK1(setRange, void(aleatoric::Range newRange));
    MAKE_MOCK0(getRange, aleatoric::Range());
};

#endif /* NumberProtocolMock_hpp */
