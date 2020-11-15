#include "DiscreteGenerator.hpp"

#include "Engine.hpp"

namespace aleatoric {
DiscreteGenerator::DiscreteGenerator() : m_engine(std::make_unique<Engine>())
{
    setDistributionVector(std::vector<double> {1.0, 1.0});
}

DiscreteGenerator::DiscreteGenerator(std::vector<double> distributionVector)
: m_engine(std::make_unique<Engine>())
{
    setDistributionVector(distributionVector);
}

DiscreteGenerator::DiscreteGenerator(int vectorSize, double uniformValue)
: m_engine(std::make_unique<Engine>())
{
    setDistributionVector(vectorSize, uniformValue);
}

DiscreteGenerator::~DiscreteGenerator()
{}

int DiscreteGenerator::getNumber()
{
    return m_distribution(m_engine->getEngine());
}

void DiscreteGenerator::setDistributionVector(
    std::vector<double> distributionVector)
{
    m_distributionVector = distributionVector;
    setDistribution();
}

void DiscreteGenerator::setDistributionVector(int vectorSize,
                                              double uniformValue)
{
    m_distributionVector.clear();
    for(int i = 0; i < vectorSize; i++) {
        m_distributionVector.push_back(uniformValue);
    }
    setDistribution();
}

void DiscreteGenerator::updateDistributionVector(int index, double newValue)
{
    m_distributionVector[index] = newValue;
    setDistribution();
}

void DiscreteGenerator::updateDistributionVector(double uniformValue)
{
    for(auto &&i : m_distributionVector) {
        i = uniformValue;
    }
    setDistribution();
}

std::vector<double> DiscreteGenerator::getDistributionVector()
{
    return m_distributionVector;
}

void DiscreteGenerator::setDistribution()
{
    m_distribution =
        std::discrete_distribution<int>(m_distributionVector.begin(),
                                        m_distributionVector.end());
}
} // namespace aleatoric
