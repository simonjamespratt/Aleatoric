//
//  NumberGenerators.cpp
//  AleatoricCompositionTools
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#include "NumberGenerators.hpp"

using namespace actlib::NumberGenerators;

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

// SERIES RANDOM =====================================================
// range is inclusive, so range is: end - start + 1
Serial::Serial(std::mt19937& engine, int rangeStart, int rangeEnd) :
    NumberGeneratorInterface(rangeStart, rangeEnd),
    generator(engine, range.size, 1.0)
{}

Serial::~Serial()
{
}

int Serial::getNumber()
{
    if (seriesIsComplete())
    {
        // Set all vector values back to uniform (equal probability)
        generator.updateDistributionVector(1.0);
    }

    int selectedNumber = generator.getNumber();
    generator.updateDistributionVector(selectedNumber, 0.0);
    return selectedNumber + range.offset;
}

bool Serial::seriesIsComplete()
{
    auto distributionVector = generator.getDistributionVector();
    for (auto &&item : distributionVector)
    {
        if (item > 0.0)
        {
            return false;
        }
    }
    return true;
}
