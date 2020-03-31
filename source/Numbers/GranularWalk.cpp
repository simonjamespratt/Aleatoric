#include "GranularWalk.hpp"

#include "Utilities.hpp"

#include <math.h>
#include <stdexcept> // std::invalid_argument
#include <string>

namespace actlib { namespace Numbers { namespace Granular {
GranularWalk::GranularWalk(std::unique_ptr<IUniformGenerator> generator,
                           std::unique_ptr<Range> range,
                           double deviationFactor)
: m_externalRange(std::move(range)),
  m_generator(std::move(generator)),
  m_haveInitialSelection(false),
  m_haveRequestedFirstNumber(false)
{
    if(deviationFactor < 0.0 || deviationFactor > 1.0) {
        throw std::invalid_argument(
            "The value passed as argument for deviationFactor must be within "
            "the range of 0.0 to 1.0");
    }

    m_internalRange = {0, 65000, 0};
    double maxStep = scaleToRange(deviationFactor,
                                  m_internalRange.start,
                                  m_internalRange.end);
    m_internalRange.maxStep = static_cast<int>(round(maxStep));

    m_generator->setDistribution(m_internalRange.start, m_internalRange.end);
}

GranularWalk::GranularWalk(std::unique_ptr<IUniformGenerator> generator,
                           std::unique_ptr<Range> range,
                           double deviationFactor,
                           int initialSelection)
: GranularWalk(std::move(generator), std::move(range), deviationFactor)
{
    if(initialSelection < m_externalRange->start ||
       initialSelection > m_externalRange->end) {
        throw std::invalid_argument(
            "The value passed as argument for initialSelection must be "
            "within the range of " +
            std::to_string(m_externalRange->start) + " to " +
            std::to_string(m_externalRange->end));
    }

    m_initialSelection = initialSelection;
    m_haveInitialSelection = true;
}

GranularWalk::~GranularWalk()
{}

double GranularWalk::getNumber()
{
    if(m_haveInitialSelection && !m_haveRequestedFirstNumber) {
        m_haveRequestedFirstNumber = true;

        auto normalized = normalize(m_initialSelection,
                                    m_externalRange->start,
                                    m_externalRange->end);

        auto scaled = scaleToRange(normalized,
                                   m_internalRange.start,
                                   m_internalRange.end);

        setForNextStep(static_cast<int>(round(scaled)));

        return static_cast<double>(m_initialSelection);
    }

    auto selectedNumber = m_generator->getNumber();
    setForNextStep(selectedNumber);
    auto normalized =
        normalize(selectedNumber, m_internalRange.start, m_internalRange.end);
    return scaleToRange(normalized,
                        m_externalRange->start,
                        m_externalRange->end);
}

void GranularWalk::reset()
{
    m_generator->setDistribution(m_internalRange.start, m_internalRange.end);
    m_haveRequestedFirstNumber = false;
}

double
GranularWalk::scaleToRange(double normalizedValue, int rangeMin, int rangeMax)
{
    return (normalizedValue * (rangeMax - rangeMin)) + rangeMin;
}

double GranularWalk::normalize(int value, int rangeMin, int rangeMax)
{
    return static_cast<double>(value - rangeMin) /
           static_cast<double>(rangeMax - rangeMin);
}

void GranularWalk::setForNextStep(int lastSelectedNumber)
{
    auto newSubRange =
        actlib::Utilities::getMaxStepSubRange(lastSelectedNumber,
                                              m_internalRange.maxStep,
                                              m_internalRange.start,
                                              m_internalRange.end);

    m_generator->setDistribution(std::get<0>(newSubRange),
                                 std::get<1>(newSubRange));
}

}}} // namespace actlib::Numbers::Granular
