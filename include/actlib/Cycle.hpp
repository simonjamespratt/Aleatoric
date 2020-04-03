#ifndef Cycle_hpp
#define Cycle_hpp

#include "ProtocolSteps.hpp"
#include "Range.hpp"

#include <memory>

namespace actlib { namespace Numbers { namespace Steps {
class Cycle : public Protocol {
  public:
    Cycle(std::unique_ptr<Range> range,
          bool bidirectional = false,
          bool reverseDirection = false);

    Cycle(std::unique_ptr<Range> range,
          int initialSelction,
          bool bidirectional = false,
          bool reverseDirection = false);

    ~Cycle();

    int getNumber() override;
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
}}} // namespace actlib::Numbers::Steps

#endif /* Cycle_hpp */
