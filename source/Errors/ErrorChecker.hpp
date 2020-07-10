#ifndef ErrorChecker_hpp
#define ErrorChecker_hpp

#include "Range.hpp"

#include <string>

namespace aleatoric {

class ErrorChecker {
  public:
    ErrorChecker();
    ~ErrorChecker();

    static void checkInitialSelectionInRange(int initialSelection,
                                             Range const &range);

    // In mathematics, the unit interval is the closed interval [0,1], that is,
    // the set of all real numbers that are greater than or equal to 0 and less
    // than or equal to 1. See https://en.wikipedia.org/wiki/Unit_interval
    static void checkValueWithinUnitInterval(double value,
                                             std::string argumentName);
};

} // namespace aleatoric

#endif /* ErrorChecker_hpp */
