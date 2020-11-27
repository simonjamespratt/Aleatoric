#include "SeriesPrinciple.hpp"

namespace aleatoric {
SeriesPrinciple::SeriesPrinciple()
{}

SeriesPrinciple::~SeriesPrinciple()
{}

int SeriesPrinciple::getNumber(std::unique_ptr<IDiscreteGenerator> &generator)
{
    int selectedNumber = generator->getNumber();
    generator->updateDistributionVector(selectedNumber, 0.0);
    return selectedNumber;
}

bool SeriesPrinciple::seriesIsComplete(
    std::unique_ptr<IDiscreteGenerator> &generator)
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
    std::unique_ptr<IDiscreteGenerator> &generator)
{
    generator->updateDistributionVector(1.0);
}
}
