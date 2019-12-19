#include "DiscreteGenerator.hpp"

namespace actlib { namespace Numbers {
    DiscreteGenerator::DiscreteGenerator(std::mt19937& engine, std::vector<double> distributionVector) :
        _engine(engine)
    {
        setDistributionVector(distributionVector);
    }

    DiscreteGenerator::DiscreteGenerator(std::mt19937& engine, int vectorSize, double uniformValue) :
        _engine(engine)
    {
        setDistributionVector(vectorSize, uniformValue);
    }

    DiscreteGenerator::~DiscreteGenerator() {}

    int DiscreteGenerator::getNumber() {
        return _distribution(_engine);
    }

    void DiscreteGenerator::setDistributionVector(std::vector<double> distributionVector) {
        _distributionVector = distributionVector;
        setDistribution();
    }

    void DiscreteGenerator::setDistributionVector(int vectorSize, double uniformValue) {
        _distributionVector.clear();
        for (int i = 0; i < vectorSize; i++)
        {
            _distributionVector.push_back(uniformValue);
        }
        setDistribution();
    }

    void DiscreteGenerator::updateDistributionVector(int index, double newValue) {
        _distributionVector[index] = newValue;
        setDistribution();
    }

    void DiscreteGenerator::updateDistributionVector(double uniformValue) {
        for (auto &&i : _distributionVector)
        {
            i = uniformValue;
        }
        setDistribution();

    }

    std::vector<double> DiscreteGenerator::getDistributionVector() {
        return _distributionVector;
    }

    void DiscreteGenerator::setDistribution() {
        _distribution = std::discrete_distribution<int>(_distributionVector.begin(), _distributionVector.end());
    }
}}
