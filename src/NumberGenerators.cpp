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
NumberGenerator::NumberGenerator(int rangeStart, int rangeEnd)
{
    range.start = rangeStart;
    range.end = rangeEnd;
    range.offset = rangeStart;
    range.size = rangeEnd - rangeStart + 1;
}

NumberGenerator::~NumberGenerator() {}

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
    discreteDistribution(random_number::engine);
}

void DiscreteNumberGenerator::updateDistribution(std::vector<double> distribution)
{
    discreteDistribution = std::discrete_distribution<int>(distribution.begin(), distribution.end());
}
