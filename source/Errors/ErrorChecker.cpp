#include "ErrorChecker.hpp"

#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib {
ErrorChecker::ErrorChecker()
{}
ErrorChecker::~ErrorChecker()
{}

void ErrorChecker::checkInitialSelectionInRange(
    int initialSelection, actlib::Numbers::Range const &range)
{
    if(initialSelection < range.start || initialSelection > range.end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(range.start) + " to " + std::to_string(range.end));
    }
}
} // namespace actlib
