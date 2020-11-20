#ifndef Geometric_hpp
#define Geometric_hpp

#include "DurationProtocol.hpp"
#include "Range.hpp"

namespace aleatoric {
class Geometric : public DurationProtocol {
  public:
    Geometric(const Range &range, int collectionSize);
    ~Geometric();
    int getCollectionSize() override;
    int getDuration(int index) override;

  private:
    std::vector<int> m_durations;
    const Range &m_range;
};
} // namespace aleatoric
#endif /* Geometric_hpp */
