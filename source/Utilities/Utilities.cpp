#include "Utilities.hpp"

namespace actlib { namespace Utilities {

std::tuple<int, int> getMaxStepSubRange(int lastSelectedNumber,
                                        int maxStep,
                                        int rangeStart,
                                        int rangeEnd)
{
    auto newRangeStart = lastSelectedNumber - maxStep;
    if(numberIsOutOfRange(newRangeStart, rangeStart, rangeEnd)) {
        newRangeStart = rangeStart;
    }

    auto newRangeEnd = lastSelectedNumber + maxStep;
    if(numberIsOutOfRange(newRangeEnd, rangeStart, rangeEnd)) {
        newRangeEnd = rangeEnd;
    }

    return std::make_tuple(newRangeStart, newRangeEnd);
};

bool numberIsOutOfRange(int number, int rangeStart, int rangeEnd)
{
    return number < rangeStart || number > rangeEnd;
};

// Series Principle Class Definitions
SeriesPrinciple::SeriesPrinciple()
{}

SeriesPrinciple::~SeriesPrinciple()
{}

int SeriesPrinciple::getNumber(
    std::unique_ptr<actlib::Numbers::IDiscreteGenerator> &generator)
{
    int selectedNumber = generator->getNumber();
    generator->updateDistributionVector(selectedNumber, 0.0);
    return selectedNumber;
}

bool SeriesPrinciple::seriesIsComplete(
    std::unique_ptr<actlib::Numbers::IDiscreteGenerator> &generator)
{
    auto distributionVector = generator->getDistributionVector();
    for(auto &&item : distributionVector) {
        if(item > 0.0) {
            return false;
        }
    }
    return true;
}

void SeriesPrinciple::resetSeries(
    std::unique_ptr<actlib::Numbers::IDiscreteGenerator> &generator)
{
    generator->updateDistributionVector(1.0);
}

}} // namespace actlib::Utilities
