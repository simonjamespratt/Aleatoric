#include "Utilities.hpp"

namespace actlib { namespace Utilities {

std::tuple<int, int> getMaxStepSubRange(int lastSelectedNumber,
                                        int maxStep,
                                        int rangeStart,
                                        int rangeEnd)
{
    auto newRangeStart = lastSelectedNumber - maxStep;
    if(numberIsOutOfRange(newRangeStart, rangeStart, rangeEnd)) {
        newRangeStart = rangeStart;
    }

    auto newRangeEnd = lastSelectedNumber + maxStep;
    if(numberIsOutOfRange(newRangeEnd, rangeStart, rangeEnd)) {
        newRangeEnd = rangeEnd;
    }

    return std::make_tuple(newRangeStart, newRangeEnd);
};

bool numberIsOutOfRange(int number, int rangeStart, int rangeEnd)
{
    return number < rangeStart || number > rangeEnd;
};

}} // namespace actlib::Utilities
