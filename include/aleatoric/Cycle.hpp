#ifndef Cycle_hpp
#define Cycle_hpp

#include "CycleStates.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class Cycle : public NumberProtocol {
  public:
    Cycle(Range range,
          bool bidirectional = false,
          bool reverseDirection = false);

    ~Cycle();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

    void setRange(Range newRange) override;

    Range getRange() override;

  private:
    Range m_range;
    std::unique_ptr<CycleState> m_state;
    void setState(bool bidirectional, bool reverseDirection);
    int m_nextPosition;
    bool m_haveRequestedFirstNumber;
};
} // namespace aleatoric

#endif /* Cycle_hpp */
