#ifndef Cycle_hpp
#define Cycle_hpp

#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class CycleState;

class Cycle : public NumberProtocol {
  public:
    Cycle();

    Cycle(Range range,
          bool bidirectional = false,
          bool reverseDirection = false);

    ~Cycle();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

  private:
    Range m_range;
    bool m_bidirectional;
    bool m_reverseDirection;
    std::unique_ptr<CycleState> m_state;
    void setState();
    void setRange(Range newRange);
    void initialise();
    int m_nextPosition;
    int m_lastPosition;
    bool m_haveRequestedFirstNumber;
};
} // namespace aleatoric

#endif /* Cycle_hpp */
