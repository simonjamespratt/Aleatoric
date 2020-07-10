#ifndef Cycle_hpp
#define Cycle_hpp

#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class Cycle : public NumberProtocol {
  public:
    Cycle(std::unique_ptr<Range> range,
          bool bidirectional = false,
          bool reverseDirection = false);

    Cycle(std::unique_ptr<Range> range,
          int initialSelction,
          bool bidirectional = false,
          bool reverseDirection = false);

    ~Cycle();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

  private:
    std::unique_ptr<Range> m_range;
    int m_position;
    bool m_bidirectional;
    bool m_reverse;
    bool m_initialStateReverse;
    int m_initialSelection;
    bool m_hasInitialSelection;
};
} // namespace aleatoric

#endif /* Cycle_hpp */
