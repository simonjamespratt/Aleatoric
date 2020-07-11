#include "Range.hpp"

namespace aleatoric {

Range::Range(int rangeStart, int rangeEnd)
{
    start = rangeStart;
    end = rangeEnd;
    offset = rangeStart;
    size = rangeEnd - rangeStart + 1;
}
} // namespace aleatoric
