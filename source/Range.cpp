#include "Range.hpp"

#include <stdexcept>

namespace aleatoric {

Range::Range(int rangeStart, int rangeEnd)
{
    if(rangeEnd <= rangeStart) {
        throw std::invalid_argument(
            "The supplied range end must be greater than the range start");
    }

    start = rangeStart;
    end = rangeEnd;
    offset = rangeStart;
    size = rangeEnd - rangeStart + 1;
}

bool Range::numberIsInRange(int number) const
{
    return !(number < start || number > end);
}
} // namespace aleatoric
