#include "Prescribed.hpp"

#include <stdexcept>

namespace actlib { namespace TimeDomain {
Prescribed::Prescribed(const std::vector<int> &durations)
: m_durations(durations)
{
    for(auto &&i : m_durations) {
        if(i < 1) {
            throw std::invalid_argument(
                "All durations supplied must be equal to, or greater than, 1");
        }
    }
}

Prescribed::~Prescribed()
{}

int Prescribed::getCollectionSize()
{
    return m_durations.size();
}

int Prescribed::getDuration(int index)
{
    // NB: using vector.at() because it will throw out of bounds if index isn't
    // accessible
    return m_durations.at(index);
}

}} // namespace actlib::TimeDomain
