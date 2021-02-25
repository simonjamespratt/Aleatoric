#ifndef Geometric_hpp
#define Geometric_hpp

#include "DurationProtocol.hpp"
#include "Range.hpp"

namespace aleatoric {
class Geometric : public DurationProtocol {
  public:
    Geometric(Range range, int collectionSize);
    ~Geometric();
    int getCollectionSize() override;
    int getDuration(int index) override;
    std::vector<int> getSelectableDurations() override;

  private:
    std::vector<int> m_durations;
    Range m_range;
};
} // namespace aleatoric
#endif /* Geometric_hpp */
