#include "Multiples.hpp"

#include "ErrorChecker.hpp"

#include <math.h>
#include <stdexcept>

namespace aleatoric {
Multiples::Multiples(int baseIncrement, Range range)
{
    if(baseIncrement < 1) {
        throw std::invalid_argument(
            "The base increment supplied must be equal to, or greater than, 1");
    }

    if(range.start < 1) {
        throw std::invalid_argument(
            "The range object supplied must have a start value equal to, or "
            "greater than, 1");
    }

    for(int i = range.start; i <= range.end; i++) {
        m_durations.push_back(i * baseIncrement);
    }

    m_hasDeviationFactor = false;
}

Multiples::Multiples(int baseIncrement,
                     Range range,
                     double deviationFactor,
                     std::unique_ptr<IUniformGenerator> generator)
: Multiples(baseIncrement, range)
{
    ErrorChecker::checkValueWithinUnitInterval(deviationFactor,
                                               "deviationFactor");

    m_deviationFactor = deviationFactor;
    m_hasDeviationFactor = true;
    m_generator = std::move(generator);
}

Multiples::Multiples(int baseIncrement, std::vector<int> multipliers)
{
    if(baseIncrement < 1) {
        throw std::invalid_argument(
            "The base increment supplied must be equal to, or greater than, 1");
    }

    for(auto &&i : multipliers) {
        if(i < 1) {
            throw std::invalid_argument(
                "The collection passed for the argument multipliers contains "
                "an invalid value. Values must be equal to, or greater than, "
                "1");
        }

        m_durations.push_back(i * baseIncrement);
    }

    m_hasDeviationFactor = false;
}

Multiples::Multiples(int baseIncrement,
                     std::vector<int> multipliers,
                     double deviationFactor,
                     std::unique_ptr<IUniformGenerator> generator)
: Multiples(baseIncrement, multipliers)
{
    ErrorChecker::checkValueWithinUnitInterval(deviationFactor,
                                               "deviationFactor");

    m_deviationFactor = deviationFactor;
    m_hasDeviationFactor = true;
    m_generator = std::move(generator);
}

Multiples::~Multiples()
{}

int Multiples::getCollectionSize()
{
    return m_durations.size();
}

int Multiples::getDuration(int index)
{
    // NB: using vector.at() because it will throw out of bounds if index isn't
    // accessible

    if(m_hasDeviationFactor) {
        // NB: The deviationFactor is the FULL value EITHER SIDE of the selected
        // duration, NOT HALF either side! This is in line the way that
        // GranularWalk handles a deviationFactor

        // calculate the min-max devFactor around the selected duration
        // e.g. duration = 100, devFactor 0.5 = min: 50, max: 150
        int potentialDeviation =
            static_cast<int>(round(m_deviationFactor * m_durations.at(index)));

        // round the min-max values and cast to ints
        int deviationMin = m_durations.at(index) - potentialDeviation;
        int deviationMax = m_durations.at(index) + potentialDeviation;

        // set the generator to the right range
        m_generator->setDistribution(deviationMin, deviationMax);

        // get a number and return it
        return m_generator->getNumber();
    }

    return m_durations.at(index);
}

std::vector<int> Multiples::getSelectableDurations()
{
    return m_durations;
}

} // namespace aleatoric
