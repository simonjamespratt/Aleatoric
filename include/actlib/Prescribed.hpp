#ifndef Prescribed_hpp
#define Prescribed_hpp

#include "DurationProtocol.hpp"

namespace actlib { namespace TimeDomain {
class Prescribed : public DurationProtocol {
  public:
    Prescribed(const std::vector<int> &durations);
    ~Prescribed();
    int getCollectionSize() override;
    int getDuration(int index) override;

  private:
    const std::vector<int> &m_durations;
};
}}     // namespace actlib::TimeDomain
#endif /* Prescribed_hpp */
