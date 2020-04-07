#include "Precision.hpp"

#include <numeric>   // std::accumulate
#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers { namespace Steps {
Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     std::unique_ptr<Range> range,
                     std::vector<double> distribution)
: m_generator(std::move(generator)), m_range(std::move(range))
{
    double sumDistValues =
        std::accumulate(distribution.begin(), distribution.end(), 0.0);
    if(sumDistValues != 1.0) {
        throw std::invalid_argument(
            "The sum of the values provided as the vector for the "
            "distribution must equal 1.0");
    }

    if(distribution.size() != m_range->size) {
        throw std::invalid_argument("The vector size for the distribution must "
                                    "match the size of the provided range");
    }

    m_generator->setDistributionVector(distribution);
    m_haveInitialSelection = false;
}

Precision::Precision(std::unique_ptr<IDiscreteGenerator> generator,
                     std::unique_ptr<Range> range,
                     std::vector<double> distribution,
                     int initialSelection)
: Precision(std::move(generator), std::move(range), distribution)
{
    if(initialSelection < m_range->start || initialSelection > m_range->end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(m_range->start) + " to " +
            std::to_string(m_range->end));
    }
    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

Precision::~Precision()
{}

int Precision::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        m_haveRequestedFirstNumber = true;
        return m_initialSelection;
    }

    return m_generator->getNumber() + m_range->offset;
}

void Precision::reset()
{
    if(m_haveInitialSelection) {
        m_haveRequestedFirstNumber = false;
    }
}
}}} // namespace actlib::Numbers::Steps
