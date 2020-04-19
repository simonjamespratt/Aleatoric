#ifndef DurationProtocol_hpp
#define DurationProtocol_hpp

#include <vector>

namespace actlib { namespace TimeDomain {

class DurationProtocol {
  public:
    virtual int getCollectionSize() = 0;

    virtual int getDuration(int index) = 0;

    virtual ~DurationProtocol() = default;
};
}} // namespace actlib::TimeDomain

#endif /* DurationProtocol_hpp */
