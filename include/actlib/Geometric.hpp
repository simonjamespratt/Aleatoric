#ifndef Geometric_hpp
#define Geometric_hpp

#include "DurationProtocol.hpp"
#include "Range.hpp"

namespace actlib { namespace TimeDomain {
class Geometric : public DurationProtocol {
  public:
    Geometric(const actlib::Numbers::Range &range, int collectionSize);
    ~Geometric();
    int getCollectionSize() override;
    int getDuration(int index) override;

  private:
    std::vector<int> m_durations;
    const actlib::Numbers::Range &m_range;
};
}}     // namespace actlib::TimeDomain
#endif /* Geometric_hpp */
