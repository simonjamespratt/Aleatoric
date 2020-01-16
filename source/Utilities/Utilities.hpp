#ifndef Utilities_hpp
#define Utilities_hpp

#include <tuple>

namespace actlib { namespace Utilities {

// Used by protocol implementations that have a max step sub range limiter for
// number selection. For example, is used by Walk and GranularWalk
std::tuple<int, int> getMaxStepSubRange(int lastSelectedNumber,
                                        int maxStep,
                                        int rangeStart,
                                        int rangeEnd);

// Range is inclusive
bool numberIsOutOfRange(int number, int rangeStart, int rangeEnd);

}} // namespace actlib::Utilities

#endif /* Utilities_hpp */
