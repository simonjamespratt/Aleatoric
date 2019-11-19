//
//  NumberGenerators.cpp
//  AleatoricCompositionTools
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#include "NumberGenerators.hpp"

using namespace actlib::NumberGenerators;

namespace random_number
{
    std::random_device rd;
    std::mt19937 engine(rd());
} // namespace random_number

// Number Generator ===========================================
// range is inclusive, so range is: end - start + 1
NumberGeneratorInterface::NumberGeneratorInterface(int rangeStart, int rangeEnd)
{
    range.start = rangeStart;
    range.end = rangeEnd;
    range.offset = rangeStart;
    range.size = rangeEnd - rangeStart + 1;
}

NumberGeneratorInterface::~NumberGeneratorInterface() {}

// UniformNumberGenerator ====================================
UniformNumberGenerator::UniformNumberGenerator(int rangeStart, int rangeEnd) : uniformDistribution(rangeStart, rangeEnd)
{}

UniformNumberGenerator::~UniformNumberGenerator() {}

int UniformNumberGenerator::getNumber()
{
    return uniformDistribution(random_number::engine);
}

// DiscreteNumberGenerator ===================================
DiscreteNumberGenerator::DiscreteNumberGenerator() {}
DiscreteNumberGenerator::DiscreteNumberGenerator(std::vector<double> distribution) : discreteDistribution(distribution.begin(), distribution.end())
{}

DiscreteNumberGenerator::~DiscreteNumberGenerator() {}

int DiscreteNumberGenerator::getNumber()
{
    return discreteDistribution(random_number::engine);
}

void DiscreteNumberGenerator::updateDistribution(std::vector<double> distribution)
{
    discreteDistribution = std::discrete_distribution<int>(distribution.begin(), distribution.end());
}

// SERIES RANDOM =====================================================
// range is inclusive, so range is: end - start + 1
Serial::Serial(int rangeStart, int rangeEnd) : NumberGeneratorInterface(rangeStart, rangeEnd)
{
    setEqualProbability();
}

Serial::~Serial()
{
}

int Serial::getNumber()
{
    if (seriesIsComplete())
    {
        setEqualProbability();
    }

    int selectedNumber = generator.getNumber();
    distributionArray[selectedNumber] = 0.0;
    generator.updateDistribution(distributionArray);
    return selectedNumber + range.offset;
}

void Serial::setEqualProbability()
{
    distributionArray.clear();
    for (int i = 0; i < range.size; i++)
    {
        distributionArray.push_back(1.0);
    }
    generator.updateDistribution(distributionArray);
}

bool Serial::seriesIsComplete()
{
    for (auto &&item : distributionArray)
    {
        if (item > 0.0)
        {
            return false;
        }
    }
    return true;
}
