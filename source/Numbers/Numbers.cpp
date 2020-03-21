#include "Numbers.hpp"

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "NoRepetition.hpp"
#include "Periodic.hpp"
#include "Range.hpp"
#include "Serial.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

namespace actlib { namespace Numbers {
Numbers::Numbers() {};
Numbers::~Numbers() {};

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createAdjacentSteps(int rangeStart, int rangeEnd)
{
    return std::make_unique<actlib::Numbers::Steps::AdjacentSteps>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd));
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createAdjacentSteps(int rangeStart, int rangeEnd, int initialSelection)
{
    return std::make_unique<actlib::Numbers::Steps::AdjacentSteps>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        initialSelection);
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createBasic(int rangeStart, int rangeEnd)
{
    return std::make_unique<actlib::Numbers::Steps::Basic>(
        std::make_unique<actlib::Numbers::UniformGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd));
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createNoRepetition(int rangeStart, int rangeEnd)
{
    return std::make_unique<actlib::Numbers::Steps::NoRepetition>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd));
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createPeriodic(int rangeStart, int rangeEnd, double chanceOfRepetition)
{
    return std::make_unique<actlib::Numbers::Steps::Periodic>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        chanceOfRepetition);
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createPeriodic(int rangeStart,
                        int rangeEnd,
                        double chanceOfRepetition,
                        int initialSelection)
{
    return std::make_unique<actlib::Numbers::Steps::Periodic>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        chanceOfRepetition,
        initialSelection);
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createSerial(int rangeStart, int rangeEnd)
{
    return std::make_unique<actlib::Numbers::Steps::Serial>(
        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd));
}

std::unique_ptr<actlib::Numbers::Steps::Protocol>
Numbers::createWalk(int rangeStart, int rangeEnd, int maxStep)
{
    return std::make_unique<actlib::Numbers::Steps::Walk>(
        std::make_unique<actlib::Numbers::UniformGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        maxStep);
}

std::unique_ptr<actlib::Numbers::Steps::Protocol> Numbers::createWalk(
    int rangeStart, int rangeEnd, int maxStep, int initialSelection)
{
    return std::make_unique<actlib::Numbers::Steps::Walk>(
        std::make_unique<actlib::Numbers::UniformGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        maxStep,
        initialSelection);
}

std::unique_ptr<actlib::Numbers::Granular::Protocol>
Numbers::createGranularWalk(int rangeStart,
                            int rangeEnd,
                            double deviationFactor)
{
    return std::make_unique<actlib::Numbers::Granular::GranularWalk>(
        std::make_unique<actlib::Numbers::UniformGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        deviationFactor);
}

std::unique_ptr<actlib::Numbers::Granular::Protocol>
Numbers::createGranularWalk(int rangeStart,
                            int rangeEnd,
                            double deviationFactor,
                            int initialSelection)
{
    return std::make_unique<actlib::Numbers::Granular::GranularWalk>(
        std::make_unique<actlib::Numbers::UniformGenerator>(),
        std::make_unique<actlib::Numbers::Range>(rangeStart, rangeEnd),
        deviationFactor,
        initialSelection);
}
}} // namespace actlib::Numbers
