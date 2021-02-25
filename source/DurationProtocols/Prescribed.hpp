#ifndef Prescribed_hpp
#define Prescribed_hpp

#include "DurationProtocol.hpp"

namespace aleatoric {
class Prescribed : public DurationProtocol {
  public:
    Prescribed(std::vector<int> durations);
    ~Prescribed();
    int getCollectionSize() override;
    int getDuration(int index) override;
    std::vector<int> getSelectableDurations() override;

  private:
    std::vector<int> m_durations;
};
} // namespace aleatoric
#endif /* Prescribed_hpp */
