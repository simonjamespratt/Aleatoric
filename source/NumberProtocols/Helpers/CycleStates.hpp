#ifndef CycleStates_hpp
#define CycleStates_hpp

#include "Range.hpp"

namespace aleatoric {
// Interface
class CycleState {
  public:
    virtual int getPosition(int &nextPosition, const Range &range) = 0;
    virtual void setRange(const int &lastPosition,
                          int &nextPosition,
                          const Range &range,
                          const bool &haveRequestedFirstNumber) = 0;
    virtual ~CycleState() = default;
};

// Implementations
class UniForward : public CycleState {
  public:
    UniForward();
    int getPosition(int &nextPosition, const Range &range) override;
    void setRange(const int &lastPosition,
                  int &nextPosition,
                  const Range &range,
                  const bool &haveRequestedFirstNumber) override;
};

class UniReverse : public CycleState {
  public:
    UniReverse();
    int getPosition(int &nextPosition, const Range &range) override;
    void setRange(const int &lastPosition,
                  int &nextPosition,
                  const Range &range,
                  const bool &haveRequestedFirstNumber) override;
};

class Bidirectional : public CycleState {
  public:
    Bidirectional(bool initialStateReverse);
    int getPosition(int &nextPosition, const Range &range) override;
    void setRange(const int &lastPosition,
                  int &nextPosition,
                  const Range &range,
                  const bool &haveRequestedFirstNumber) override;

  private:
    bool m_reverse;
    bool m_initialStateReverse;
};

} // namespace aleatoric
#endif /* CycleStates_hpp */
