#include "Range.hpp"

namespace actlib { namespace Numbers { namespace Steps {
Range::Range(int rangeStart, int rangeEnd)
{
    start = rangeStart;
    end = rangeEnd;
    offset = rangeStart;
    size = rangeEnd - rangeStart + 1;
}
}}} // namespace actlib::Numbers
