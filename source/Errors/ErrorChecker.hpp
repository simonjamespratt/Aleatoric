#ifndef ErrorChecker_hpp
#define ErrorChecker_hpp

#include "Range.hpp"

namespace actlib {

class ErrorChecker {
  public:
    ErrorChecker();
    ~ErrorChecker();

    static void
    checkInitialSelectionInRange(int initialSelection,
                                 actlib::Numbers::Range const &range);
};

} // namespace actlib

#endif /* ErrorChecker_hpp */
