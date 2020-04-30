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
    return m_durations[index];
}

}} // namespace actlib::TimeDomain
