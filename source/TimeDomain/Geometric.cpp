#include "Geometric.hpp"

#include <math.h>
#include <stdexcept>

namespace actlib { namespace TimeDomain {
Geometric::Geometric(const actlib::Numbers::Range &range, int collectionSize)
: m_durations(collectionSize), m_range(range)
{
    if(m_range.start < 1) {
        throw std::invalid_argument("The range object supplied must have a "
                                    "start value equal to, or greater than, 1");
    }

    // calculate the common ratio:
    // pow((m_range.end / m_range.start), (1 / collectionSize - 1))
    double crBase =
        static_cast<double>(m_range.end) / static_cast<double>(m_range.start);

    double crExponent = 1.0 / (m_durations.size() - 1);

    double commonRatio = pow(crBase, crExponent);

    // populate m_durations with the geometric sequence
    // each index in m_durations corresponds to a term in the geometric sequence
    // which raises the common ratio to the power of that index.
    // the result is then scaled to the range by multiplying it by the range
    // start/offset (called the scale factor). For further details,
    // see https://en.wikipedia.org/wiki/Geometric_progression
    for(int i = 0; i < m_durations.size(); i++) {
        m_durations[i] =
            static_cast<int>(round(m_range.offset * pow(commonRatio, i)));
    }
}

Geometric::~Geometric()
{}

int Geometric::getCollectionSize()
{
    return m_durations.size();
}

int Geometric::getDuration(int index)
{
    // NB: using vector.at() because it will throw out of bounds if index isn't
    // accessible

    return m_durations.at(index);
}
}} // namespace actlib::TimeDomain
