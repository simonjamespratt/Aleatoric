#include "DiscreteGenerator.hpp"

namespace actlib { namespace Numbers {
DiscreteGenerator::DiscreteGenerator(std::vector<double> distributionVector)
: m_engine(m_seedSource)
{
    setDistributionVector(distributionVector);
}

DiscreteGenerator::DiscreteGenerator(int vectorSize, double uniformValue)
: m_engine(m_seedSource)
{
    setDistributionVector(vectorSize, uniformValue);
}

DiscreteGenerator::~DiscreteGenerator()
{}

int DiscreteGenerator::getNumber()
{
    return m_distribution(m_engine);
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
}} // namespace actlib::Numbers
